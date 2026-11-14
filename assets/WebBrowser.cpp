#include <webkit2/webkit2.h>
#include <gtk/gtk.h>
#include <cstdio>
#include <cstring>
#include <string>
#include <sys/stat.h>
#include <ctime>
#include <vector>

static GtkWidget* notebook = nullptr;
static GtkWidget* url_entry = nullptr;
static GtkWidget* progress_bar = nullptr;   
static GtkWidget* status_overlay = nullptr; 
static guint status_hide_timer = 0;         
static gboolean is_typing = FALSE; 

static WebKitWebContext* web_context = nullptr;
static WebKitCookieManager* cookie_manager = nullptr;
static WebKitWebsiteDataManager* data_manager = nullptr;

// 独立数据目录
static std::string base_dir = "/data/data/com.termux/files/home/.config/WebBrowser/data";
static std::string history_file = base_dir + "/history/history.txt";
static std::string bookmark_file = base_dir + "/bookmarks/bookmarks.txt";
static std::string log_file = base_dir + "/logs/browser.log";
static guint resize_timer_id = 0;

struct RecordItem {
    std::string time;
    std::string title;
    std::string url;
};
static std::vector<RecordItem> history_list;
static std::vector<RecordItem> bookmark_list;

static GtkListStore* history_store = nullptr;
static GtkListStore* cookie_store = nullptr;
static GtkListStore* bookmark_store = nullptr;
static GtkWidget* record_dialog = nullptr;

GtkWidget* get_webview_from_page(GtkWidget* page) {
    if (!page) return nullptr;
    GList* children = gtk_container_get_children(GTK_CONTAINER(page));
    if (children) {
        GtkWidget* webview = GTK_WIDGET(children->data);
        g_list_free(children);
        return webview;
    }
    return nullptr;
}

GtkWidget* get_current_webview() {
    int page_idx = gtk_notebook_get_current_page(GTK_NOTEBOOK(notebook));
    if (page_idx == -1) return nullptr;
    GtkWidget* page = gtk_notebook_get_nth_page(GTK_NOTEBOOK(notebook), page_idx);
    return get_webview_from_page(page);
}

void write_log(const std::string& msg) {
    FILE* f = fopen(log_file.c_str(), "a");
    if (f) {
        time_t now = time(0);
        char time_buf[64];
        strftime(time_buf, sizeof(time_buf), "%Y-%m-%d %H:%M:%S", localtime(&now));
        fprintf(f, "[%s] %s\n", time_buf, msg.c_str());
        fclose(f);
    }
}

gboolean hide_status_callback(gpointer user_data) {
    gtk_widget_hide(status_overlay);
    status_hide_timer = 0;
    return G_SOURCE_REMOVE;
}

void update_status_text(const char* text, gboolean auto_hide) {
    if (!status_overlay) return;
    gtk_label_set_text(GTK_LABEL(status_overlay), text);
    gtk_widget_show(status_overlay);
    if (status_hide_timer > 0) {
        g_source_remove(status_hide_timer);
        status_hide_timer = 0; 
    }
    if (auto_hide) status_hide_timer = g_timeout_add(1000, hide_status_callback, NULL);
}

void on_progress_changed(WebKitWebView* web_view, GParamSpec* pspec, gpointer user_data) {
    if (GTK_WIDGET(web_view) == get_current_webview()) {
        double real_progress = webkit_web_view_get_estimated_load_progress(web_view);
        gtk_progress_bar_set_fraction(GTK_PROGRESS_BAR(progress_bar), real_progress);
    }
}

gboolean do_webview_zoom(gpointer user_data) {
    GtkWidget* webview = GTK_WIDGET(user_data);
    gint window_width = gtk_widget_get_allocated_width(webview);
    if (window_width > 0) {
        const double BASE_WIDTH = 1366.0; 
        double zoom = (double)window_width / BASE_WIDTH;
        if (zoom < 0.3) zoom = 0.3; 
        if (zoom > 1.0) zoom = 1.0;
        webkit_web_view_set_zoom_level(WEBKIT_WEB_VIEW(webview), zoom);
    }
    resize_timer_id = 0;
    return G_SOURCE_REMOVE;
}

void on_window_size_allocate(GtkWidget* widget, GtkAllocation* allocation, gpointer user_data) {
    GtkWidget* webview = get_current_webview();
    if (!webview) return;
    if (resize_timer_id > 0) {
        g_source_remove(resize_timer_id);
        resize_timer_id = 0;
    }
    resize_timer_id = g_timeout_add(150, do_webview_zoom, webview);
}

std::string resolve_input(const char* input) {
    std::string s(input);
    if (s.empty()) return "https://cn.bing.com";
    if (s.find(" ") == std::string::npos && s.find(".") != std::string::npos) {
        if (s.find("://") == std::string::npos) return "https://" + s;
        return s;
    }
    return "https://cn.bing.com/search?q=" + s;
}

void read_records(const std::string& file, std::vector<RecordItem>& list) {
    list.clear();
    FILE* f = fopen(file.c_str(), "r");
    if (f) {
        char buf[4096];
        while (fgets(buf, sizeof(buf), f)) {
            std::string line(buf);
            if (!line.empty() && line.back() == '\n') line.pop_back();
            size_t pos1 = line.find('|');
            size_t pos2 = line.find('|', pos1 + 1);
            if (pos1 != std::string::npos && pos2 != std::string::npos) {
                list.push_back({ line.substr(0, pos1), line.substr(pos1 + 1, pos2 - pos1 - 1), line.substr(pos2 + 1) });
            }
        }
        fclose(f);
    }
}

void write_records(const std::string& file, const std::vector<RecordItem>& list) {
    FILE* f = fopen(file.c_str(), "w");
    if (f) {
        for (const auto& item : list) {
            fprintf(f, "%s|%s|%s\n", item.time.c_str(), item.title.c_str(), item.url.c_str());
        }
        fclose(f);
    }
}

void save_history(const char* url, const char* title) {
    if (!url || strstr(url, "about:blank") || strstr(url, "data:text/html")) return;
    std::string t = (title && strlen(title) > 0) ? title : url;
    for (char& c : t) { if (c == '|' || c == '\n' || c == '\r') c = ' '; }
    time_t now = time(0);
    char time_buf[64];
    strftime(time_buf, sizeof(time_buf), "%Y-%m-%d %H:%M:%S", localtime(&now));
    if (history_list.size() > 500) history_list.erase(history_list.begin());
    history_list.push_back({time_buf, t, url});
    write_records(history_file, history_list);
}

void add_tab(const char* url);

gboolean on_decide_policy(WebKitWebView* web_view, WebKitPolicyDecision* decision, WebKitPolicyDecisionType type, gpointer user_data) {
    if (type == WEBKIT_POLICY_DECISION_TYPE_NEW_WINDOW_ACTION) {
        WebKitNavigationPolicyDecision* nav_decision = WEBKIT_NAVIGATION_POLICY_DECISION(decision);
        WebKitNavigationAction* action = webkit_navigation_policy_decision_get_navigation_action(nav_decision);
        WebKitURIRequest* request = webkit_navigation_action_get_request(action);
        const gchar* uri = webkit_uri_request_get_uri(request);

        if (uri) {
            webkit_policy_decision_ignore(decision);
            add_tab(uri);
            return TRUE;
        }
    }
    return FALSE;
}

gboolean on_tree_button_press(GtkWidget* widget, GdkEventButton* event, gpointer user_data) {
    if (event->type == GDK_2BUTTON_PRESS && event->button == 1) {
        GtkTreePath* path;
        GtkTreeViewColumn* column;
        
        if (gtk_tree_view_get_path_at_pos(GTK_TREE_VIEW(widget), event->x, event->y, &path, &column, NULL, NULL)) {
            std::string col_title = gtk_tree_view_column_get_title(column);
            GtkTreeModel* model = gtk_tree_view_get_model(GTK_TREE_VIEW(widget));
            GtkTreeIter iter;
            gtk_tree_model_get_iter(model, &iter, path);

            if (col_title == "操作") {
                GtkWidget* dialog = gtk_message_dialog_new(GTK_WINDOW(record_dialog), (GtkDialogFlags)GTK_DIALOG_MODAL, GTK_MESSAGE_QUESTION, GTK_BUTTONS_YES_NO, "确定要删除这条记录吗？");
                int response = gtk_dialog_run(GTK_DIALOG(dialog));
                gtk_widget_destroy(dialog);
                
                if (response == GTK_RESPONSE_YES) {
                    if (model == GTK_TREE_MODEL(history_store)) {
                        int idx;
                        gtk_tree_model_get(model, &iter, 3, &idx, -1);
                        if (idx >= 0 && idx < (int)history_list.size()) {
                            history_list.erase(history_list.begin() + idx);
                            write_records(history_file, history_list);
                        }
                        gtk_list_store_remove(history_store, &iter);
                    } else if (model == GTK_TREE_MODEL(bookmark_store)) {
                        int idx;
                        gtk_tree_model_get(model, &iter, 3, &idx, -1);
                        if (idx >= 0 && idx < (int)bookmark_list.size()) {
                            bookmark_list.erase(bookmark_list.begin() + idx);
                            write_records(bookmark_file, bookmark_list);
                        }
                        gtk_list_store_remove(bookmark_store, &iter);
                    } else if (model == GTK_TREE_MODEL(cookie_store)) {
                        gtk_list_store_remove(cookie_store, &iter);
                    }
                }
            } else {
                gchar* url = nullptr;
                gtk_tree_model_get(model, &iter, 1, &url, -1);
                if (url) {
                    GtkWidget* webview = get_current_webview();
                    if (webview) {
                        webkit_web_view_load_uri(WEBKIT_WEB_VIEW(webview), url);
                    } else {
                        add_tab(url);
                    }
                    g_free(url);
                    gtk_dialog_response(GTK_DIALOG(record_dialog), GTK_RESPONSE_CLOSE);
                }
            }
            gtk_tree_path_free(path);
        }
    }
    return FALSE;
}

void refresh_history_ui() {
    gtk_list_store_clear(history_store);
    for (int i = history_list.size() - 1; i >= 0; i--) {
        GtkTreeIter iter;
        gtk_list_store_append(history_store, &iter);
        gtk_list_store_set(history_store, &iter, 0, history_list[i].title.c_str(), 1, history_list[i].url.c_str(), 2, "✕", 3, i, -1);
    }
}

void refresh_bookmark_ui() {
    gtk_list_store_clear(bookmark_store);
    for (int i = bookmark_list.size() - 1; i >= 0; i--) {
        GtkTreeIter iter;
        gtk_list_store_append(bookmark_store, &iter);
        gtk_list_store_set(bookmark_store, &iter, 0, bookmark_list[i].title.c_str(), 1, bookmark_list[i].url.c_str(), 2, "✕", 3, i, -1);
    }
}

void on_cookies_fetched(GObject* source_object, GAsyncResult* res, gpointer user_data) {
    GError* error = nullptr;
    GList* cookies = webkit_cookie_manager_get_all_cookies_finish(WEBKIT_COOKIE_MANAGER(source_object), res, &error);
    gtk_list_store_clear(cookie_store);
    if (cookies) {
        for (GList* l = cookies; l != nullptr; l = l->next) {
            SoupCookie* cookie = (SoupCookie*)l->data;
            const gchar* domain = soup_cookie_get_domain(cookie);
            const gchar* name = soup_cookie_get_name(cookie);
            GtkTreeIter iter;
            gtk_list_store_append(cookie_store, &iter);
            gtk_list_store_set(cookie_store, &iter, 0, domain ? domain : "", 1, name ? name : "", 2, "✕", -1);
        }
        g_list_free_full(cookies, (GDestroyNotify)soup_cookie_free);
    }
    if (error) g_error_free(error);
}

GtkWidget* create_tree_view(GtkListStore* store, const char* col1_name, const char* col2_name) {
    GtkWidget* tree = gtk_tree_view_new_with_model(GTK_TREE_MODEL(store));
    GtkCellRenderer* renderer = gtk_cell_renderer_text_new();
    GtkTreeViewColumn* col1 = gtk_tree_view_column_new_with_attributes(col1_name, renderer, "text", 0, NULL);
    gtk_tree_view_column_set_expand(col1, TRUE);
    gtk_tree_view_append_column(GTK_TREE_VIEW(tree), col1);

    renderer = gtk_cell_renderer_text_new();
    GtkTreeViewColumn* col2 = gtk_tree_view_column_new_with_attributes(col2_name, renderer, "text", 1, NULL);
    gtk_tree_view_column_set_expand(col2, TRUE);
    gtk_tree_view_append_column(GTK_TREE_VIEW(tree), col2);

    renderer = gtk_cell_renderer_text_new();
    g_object_set(renderer, "foreground", "red", "weight", PANGO_WEIGHT_BOLD, NULL);
    GtkTreeViewColumn* col3 = gtk_tree_view_column_new_with_attributes("操作", renderer, "text", 2, NULL);
    gtk_tree_view_column_set_alignment(col3, 0.5);
    gtk_tree_view_append_column(GTK_TREE_VIEW(tree), col3);

    g_signal_connect(tree, "button-press-event", G_CALLBACK(on_tree_button_press), NULL);
    return tree;
}

void on_history(GtkWidget* w, gpointer d) {
    read_records(history_file, history_list);
    read_records(bookmark_file, bookmark_list);

    record_dialog = gtk_dialog_new_with_buttons("Record", GTK_WINDOW(gtk_widget_get_toplevel(w)), (GtkDialogFlags)(GTK_DIALOG_MODAL | GTK_DIALOG_DESTROY_WITH_PARENT), "关闭", GTK_RESPONSE_CLOSE, NULL);
    gtk_window_set_default_size(GTK_WINDOW(record_dialog), 800, 500);

    GtkWidget* content = gtk_dialog_get_content_area(GTK_DIALOG(record_dialog));
    GtkWidget* notebook_dialog = gtk_notebook_new();
    gtk_box_pack_start(GTK_BOX(content), notebook_dialog, TRUE, TRUE, 0);

    GtkWidget* scroll1 = gtk_scrolled_window_new(NULL, NULL);
    history_store = gtk_list_store_new(4, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_INT);
    GtkWidget* tree1 = create_tree_view(history_store, "网页标题", "网址");
    gtk_container_add(GTK_CONTAINER(scroll1), tree1);
    gtk_notebook_append_page(GTK_NOTEBOOK(notebook_dialog), scroll1, gtk_label_new("访问记录"));
    refresh_history_ui();

    GtkWidget* scroll2 = gtk_scrolled_window_new(NULL, NULL);
    bookmark_store = gtk_list_store_new(4, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_INT);
    GtkWidget* tree2 = create_tree_view(bookmark_store, "网页标题", "网址");
    gtk_container_add(GTK_CONTAINER(scroll2), tree2);
    gtk_notebook_append_page(GTK_NOTEBOOK(notebook_dialog), scroll2, gtk_label_new("网页收藏"));
    refresh_bookmark_ui();

    GtkWidget* scroll3 = gtk_scrolled_window_new(NULL, NULL);
    cookie_store = gtk_list_store_new(3, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING);
    GtkWidget* tree3 = gtk_tree_view_new_with_model(GTK_TREE_MODEL(cookie_store));
    GtkCellRenderer* r1 = gtk_cell_renderer_text_new();
    gtk_tree_view_append_column(GTK_TREE_VIEW(tree3), gtk_tree_view_column_new_with_attributes("域名", r1, "text", 0, NULL));
    GtkCellRenderer* r2 = gtk_cell_renderer_text_new();
    gtk_tree_view_append_column(GTK_TREE_VIEW(tree3), gtk_tree_view_column_new_with_attributes("名称", r2, "text", 1, NULL));
    GtkCellRenderer* r3 = gtk_cell_renderer_text_new();
    g_object_set(r3, "foreground", "red", "weight", PANGO_WEIGHT_BOLD, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(tree3), gtk_tree_view_column_new_with_attributes("操作", r3, "text", 2, NULL));
    g_signal_connect(tree3, "button-press-event", G_CALLBACK(on_tree_button_press), NULL);
    gtk_container_add(GTK_CONTAINER(scroll3), tree3);
    gtk_notebook_append_page(GTK_NOTEBOOK(notebook_dialog), scroll3, gtk_label_new("Cookies"));
    webkit_cookie_manager_get_all_cookies(cookie_manager, NULL, on_cookies_fetched, NULL);

    GtkWidget* log_vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    GtkWidget* log_toolbar = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
    GtkWidget* btn_clear_log = gtk_button_new_with_label("清空日志");
    gtk_box_pack_start(GTK_BOX(log_toolbar), btn_clear_log, FALSE, FALSE, 5);
    gtk_box_pack_start(GTK_BOX(log_vbox), log_toolbar, FALSE, FALSE, 0);

    GtkWidget* log_scroll = gtk_scrolled_window_new(NULL, NULL);
    GtkWidget* log_textview = gtk_text_view_new();
    gtk_text_view_set_editable(GTK_TEXT_VIEW(log_textview), FALSE);
    gtk_text_view_set_monospace(GTK_TEXT_VIEW(log_textview), TRUE);
    gtk_container_add(GTK_CONTAINER(log_scroll), log_textview);
    gtk_box_pack_start(GTK_BOX(log_vbox), log_scroll, TRUE, TRUE, 0);

    std::string log_content = "";
    FILE* f_log = fopen(log_file.c_str(), "r");
    if (f_log) {
        char buf[1024];
        while (fgets(buf, sizeof(buf), f_log)) log_content += buf;
        fclose(f_log);
    }
    GtkTextBuffer* log_buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(log_textview));
    gtk_text_buffer_set_text(log_buffer, log_content.c_str(), -1);

    g_signal_connect(btn_clear_log, "clicked", G_CALLBACK(+[](GtkWidget* w, gpointer data) {
        GtkTextBuffer* buf = GTK_TEXT_BUFFER(data);
        gtk_text_buffer_set_text(buf, "", -1);
        FILE* f = fopen(log_file.c_str(), "w");
        if (f) fclose(f);
    }), log_buffer);

    gtk_notebook_append_page(GTK_NOTEBOOK(notebook_dialog), log_vbox, gtk_label_new("日志"));

    gtk_widget_show_all(record_dialog);
    gtk_dialog_run(GTK_DIALOG(record_dialog));
    gtk_widget_destroy(record_dialog);
    record_dialog = nullptr;
}

void on_bookmark(GtkWidget* w, gpointer d) {
    GtkWidget* webview = get_current_webview();
    if (!webview) return;
    const gchar* uri = webkit_web_view_get_uri(WEBKIT_WEB_VIEW(webview));
    const gchar* title = webkit_web_view_get_title(WEBKIT_WEB_VIEW(webview));
    
    if (uri) {
        std::string t = (title && strlen(title) > 0) ? title : uri;
        for (char& c : t) { if (c == '|' || c == '\n' || c == '\r') c = ' '; }
        time_t now = time(0);
        char time_buf[64];
        strftime(time_buf, sizeof(time_buf), "%Y-%m-%d %H:%M:%S", localtime(&now));
        
        for (const auto& item : bookmark_list) {
            if (item.url == uri) {
                GtkWidget* dialog = gtk_message_dialog_new(GTK_WINDOW(gtk_widget_get_toplevel(w)), (GtkDialogFlags)GTK_DIALOG_MODAL, GTK_MESSAGE_INFO, GTK_BUTTONS_OK, "该页面已收藏！");
                gtk_dialog_run(GTK_DIALOG(dialog));
                gtk_widget_destroy(dialog);
                return;
            }
        }
        bookmark_list.push_back({time_buf, t, uri});
        write_records(bookmark_file, bookmark_list);
        GtkWidget* dialog = gtk_message_dialog_new(GTK_WINDOW(gtk_widget_get_toplevel(w)), (GtkDialogFlags)GTK_DIALOG_MODAL, GTK_MESSAGE_INFO, GTK_BUTTONS_OK, "收藏成功！");
        gtk_dialog_run(GTK_DIALOG(dialog));
        gtk_widget_destroy(dialog);
    }
}

gboolean on_url_entry_focus_in(GtkWidget* widget, GdkEventFocus* event, gpointer user_data) {
    is_typing = TRUE;
    return FALSE;
}

gboolean on_url_entry_focus_out(GtkWidget* widget, GdkEventFocus* event, gpointer user_data) {
    is_typing = FALSE;
    GtkWidget* webview = get_current_webview();
    if (webview) {
        const gchar* uri = webkit_web_view_get_uri(WEBKIT_WEB_VIEW(webview));
        gtk_entry_set_text(GTK_ENTRY(url_entry), uri ? uri : "");
    }
    return FALSE;
}

void on_load_changed(WebKitWebView* web_view, WebKitLoadEvent load_event, gpointer user_data) {
    gboolean is_foreground = (GTK_WIDGET(web_view) == get_current_webview());

    if (load_event == WEBKIT_LOAD_STARTED) {
        if (is_foreground) {
            update_status_text("正在连接...", FALSE);
            gtk_widget_show(progress_bar);
            gtk_progress_bar_set_fraction(GTK_PROGRESS_BAR(progress_bar), 0.0);
        }
    } else if (load_event == WEBKIT_LOAD_REDIRECTED) {
        if (is_foreground) update_status_text("正在重定向...", FALSE);
    } else if (load_event == WEBKIT_LOAD_COMMITTED) {
        if (is_foreground) update_status_text("等待响应...", FALSE);
    } else if (load_event == WEBKIT_LOAD_FINISHED) {
        if (is_foreground) {
            gtk_progress_bar_set_fraction(GTK_PROGRESS_BAR(progress_bar), 1.0);
            g_timeout_add(500, +[](gpointer data) -> gboolean {
                gtk_widget_hide(progress_bar);
                return G_SOURCE_REMOVE;
            }, NULL);
            update_status_text("完成", TRUE);
        }
        
        const gchar* uri = webkit_web_view_get_uri(web_view);
        const gchar* title = webkit_web_view_get_title(web_view);
        save_history(uri, title);
        write_log("加载完成: " + std::string(uri ? uri : "未知"));
        
        if (is_foreground && !is_typing) {
            gtk_entry_set_text(GTK_ENTRY(url_entry), uri ? uri : "");
            do_webview_zoom(web_view); 
        }
    }
}

// ================= 彻底静默：拦截所有加载失败弹窗 =================
gboolean on_load_failed(WebKitWebView* web_view, WebKitLoadEvent load_event, const gchar* failing_uri, GError* error, gpointer user_data) {
    // 1. 忽略“插件接管加载”的假错误（媒体开始播放）
    if (g_error_matches(error, WEBKIT_PLUGIN_ERROR, WEBKIT_PLUGIN_ERROR_WILL_HANDLE_LOAD)) {
        return TRUE; 
    }
    
    // 2. 忽略缺少插件（没装 gst 插件时静默当哑巴）
    if (g_error_matches(error, WEBKIT_PLUGIN_ERROR, WEBKIT_PLUGIN_ERROR_CANNOT_FIND_PLUGIN)) {
        write_log("媒体插件缺失 (静默处理): " + std::string(error->message));
        return TRUE;
    }

    // 3. 忽略取消类错误
    if (g_error_matches(error, WEBKIT_NETWORK_ERROR, WEBKIT_NETWORK_ERROR_CANCELLED)) return TRUE;
    if (g_error_matches(error, G_IO_ERROR, G_IO_ERROR_CANCELLED)) return TRUE;
    if (error->message && (strstr(error->message, "cancelled") || strstr(error->message, "Cancelled"))) return TRUE;

    // 4. 其他真正的错误，记录下来，只在左下角状态栏提示，绝不弹窗
    write_log("加载失败 (静默处理): " + std::string(failing_uri ? failing_uri : "未知") + " 错误: " + error->message);
    
    if (GTK_WIDGET(web_view) == get_current_webview()) {
        gtk_widget_hide(progress_bar);
        update_status_text("网页加载失败", TRUE);
    }
    
    return TRUE; // 吃掉所有错误，坚决不弹窗
}
// ====================================================================

void on_web_process_terminated(WebKitWebView* web_view, WebKitWebProcessTerminationReason reason, gpointer user_data) {
    const gchar* uri = webkit_web_view_get_uri(web_view);
    if (uri) webkit_web_view_load_uri(web_view, uri);
}

void on_url_activate(GtkWidget* widget, gpointer data) {
    const char* input = gtk_entry_get_text(GTK_ENTRY(url_entry));
    std::string target = resolve_input(input);
    write_log("用户输入导航: " + target);
    is_typing = FALSE; 
    GtkWidget* webview = get_current_webview();
    if (webview) webkit_web_view_load_uri(WEBKIT_WEB_VIEW(webview), target.c_str());
}

// 核心安全机制：当标签页被销毁时，清理残留的定时器
void on_webview_destroy(GtkWidget* widget, gpointer user_data) {
    if (resize_timer_id > 0) {
        g_source_remove(resize_timer_id);
        resize_timer_id = 0;
    }
}

void add_tab(const char* url) {
    GtkWidget* webview = webkit_web_view_new_with_context(web_context);
    gtk_widget_set_hexpand(webview, TRUE);
    gtk_widget_set_vexpand(webview, TRUE);
    
    // 允许自动播放媒体（正确 API）
    WebKitSettings* settings = webkit_web_view_get_settings(WEBKIT_WEB_VIEW(webview));
    webkit_settings_set_media_playback_requires_user_gesture(settings, FALSE);

    g_signal_connect(webview, "load-changed", G_CALLBACK(on_load_changed), NULL);
    g_signal_connect(webview, "load-failed", G_CALLBACK(on_load_failed), NULL);
    g_signal_connect(webview, "web-process-terminated", G_CALLBACK(on_web_process_terminated), NULL);
    g_signal_connect(webview, "notify::estimated-load-progress", G_CALLBACK(on_progress_changed), NULL);
    g_signal_connect(webview, "decide-policy", G_CALLBACK(on_decide_policy), NULL);
    
    g_signal_connect(webview, "destroy", G_CALLBACK(on_webview_destroy), NULL);
    
    g_signal_connect(webview, "notify::title", G_CALLBACK(+[](GtkWidget* w, GParamSpec* pspec, gpointer d){
        const gchar* title = webkit_web_view_get_title(WEBKIT_WEB_VIEW(w));
        int page = gtk_notebook_get_current_page(GTK_NOTEBOOK(notebook));
        GtkWidget* current_wv = get_current_webview();
        if (w == current_wv && page != -1) {
            gtk_notebook_set_tab_label_text(GTK_NOTEBOOK(notebook), gtk_notebook_get_nth_page(GTK_NOTEBOOK(notebook), page), title ? title : "加载中...");
        }
    }), NULL);

    GtkWidget* vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_box_pack_start(GTK_BOX(vbox), webview, TRUE, TRUE, 0);
    gtk_widget_show_all(vbox);

    int page = gtk_notebook_append_page(GTK_NOTEBOOK(notebook), vbox, gtk_label_new("新标签页"));
    gtk_notebook_set_current_page(GTK_NOTEBOOK(notebook), page);
    webkit_web_view_load_uri(WEBKIT_WEB_VIEW(webview), url);
}

void on_switch_page(GtkNotebook* nb, GtkWidget* page, guint page_num, gpointer data) {
    is_typing = FALSE; 
    GtkWidget* webview = get_webview_from_page(page);
    if (webview) {
        const gchar* uri = webkit_web_view_get_uri(WEBKIT_WEB_VIEW(webview));
        gtk_entry_set_text(GTK_ENTRY(url_entry), uri ? uri : "");
        do_webview_zoom(webview);

        if (webkit_web_view_is_loading(WEBKIT_WEB_VIEW(webview))) {
            gtk_widget_show(progress_bar);
            gtk_progress_bar_set_fraction(GTK_PROGRESS_BAR(progress_bar), 
                webkit_web_view_get_estimated_load_progress(WEBKIT_WEB_VIEW(webview)));
            update_status_text("加载中...", FALSE);
        } else {
            gtk_widget_hide(progress_bar);
            update_status_text("完成", TRUE);
        }
    }
}

void on_back(GtkWidget* w, gpointer d) { GtkWidget* wv = get_current_webview(); if (wv) webkit_web_view_go_back(WEBKIT_WEB_VIEW(wv)); }
void on_forward(GtkWidget* w, gpointer d) { GtkWidget* wv = get_current_webview(); if (wv) webkit_web_view_go_forward(WEBKIT_WEB_VIEW(wv)); }
void on_refresh(GtkWidget* w, gpointer d) { GtkWidget* wv = get_current_webview(); if (wv) webkit_web_view_reload(WEBKIT_WEB_VIEW(wv)); }
void on_new_tab(GtkWidget* w, gpointer d) { add_tab("https://cn.bing.com"); }

void on_close_tab(GtkWidget* w, gpointer d) {
    int page_idx = gtk_notebook_get_current_page(GTK_NOTEBOOK(notebook));
    if (page_idx == -1) return;

    if (gtk_notebook_get_n_pages(GTK_NOTEBOOK(notebook)) <= 1) {
        return;
    }
    gtk_notebook_remove_page(GTK_NOTEBOOK(notebook), page_idx);
}

void build_ui(GtkWidget* window) {
    mkdir("/data/data/com.termux/files/home/.config", 0777);
    mkdir("/data/data/com.termux/files/home/.config/WebBrowser", 0777);
    mkdir(base_dir.c_str(), 0777);
    mkdir((base_dir + "/history").c_str(), 0777);
    mkdir((base_dir + "/bookmarks").c_str(), 0777);
    mkdir((base_dir + "/logs").c_str(), 0777);
    mkdir((base_dir + "/cookies").c_str(), 0777);
    mkdir((base_dir + "/cache").c_str(), 0777);

    read_records(history_file, history_list);
    read_records(bookmark_file, bookmark_list);
    write_log("浏览器启动");

    data_manager = webkit_website_data_manager_new(
        "base-data-directory", base_dir.c_str(),
        "base-cache-directory", (base_dir + "/cache").c_str(),
        NULL
    );

    webkit_website_data_manager_set_tls_errors_policy(data_manager, WEBKIT_TLS_ERRORS_POLICY_IGNORE);
    webkit_website_data_manager_set_network_proxy_settings(data_manager, WEBKIT_NETWORK_PROXY_MODE_NO_PROXY, NULL);

    cookie_manager = webkit_website_data_manager_get_cookie_manager(data_manager);

    webkit_cookie_manager_set_persistent_storage(
        cookie_manager,
        (base_dir + "/cookies/cookies.sqlite").c_str(),
        WEBKIT_COOKIE_PERSISTENT_STORAGE_SQLITE
    );

    web_context = webkit_web_context_new_with_website_data_manager(data_manager);

    const char* languages[] = {"zh-CN", "zh", "en", NULL};
    webkit_web_context_set_preferred_languages(web_context, languages);

    GtkWidget* vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    
    GtkWidget* toolbar = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
    gtk_container_set_border_width(GTK_CONTAINER(toolbar), 5);
    
    GtkWidget* btn_back = gtk_button_new_with_label("←");
    GtkWidget* btn_forward = gtk_button_new_with_label("→");
    GtkWidget* btn_refresh = gtk_button_new_with_label("⟳");
    GtkWidget* btn_history = gtk_button_new_with_label("◷"); 
    url_entry = gtk_entry_new();
    gtk_widget_set_hexpand(url_entry, TRUE);
    GtkWidget* btn_new_tab = gtk_button_new_with_label("+");
    GtkWidget* btn_close_tab = gtk_button_new_with_label("-"); 
    GtkWidget* btn_bookmark = gtk_button_new_with_label("☆");
    
    g_signal_connect(url_entry, "focus-in-event", G_CALLBACK(on_url_entry_focus_in), NULL);
    g_signal_connect(url_entry, "focus-out-event", G_CALLBACK(on_url_entry_focus_out), NULL);

    gtk_box_pack_start(GTK_BOX(toolbar), btn_back, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(toolbar), btn_forward, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(toolbar), btn_refresh, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(toolbar), btn_history, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(toolbar), url_entry, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(toolbar), btn_bookmark, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(toolbar), btn_new_tab, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(toolbar), btn_close_tab, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(vbox), toolbar, FALSE, FALSE, 0);

    progress_bar = gtk_progress_bar_new();
    gtk_widget_set_size_request(progress_bar, -1, 3);
    gtk_widget_hide(progress_bar);
    gtk_box_pack_start(GTK_BOX(vbox), progress_bar, FALSE, FALSE, 0);

    GtkWidget* overlay = gtk_overlay_new();
    gtk_box_pack_start(GTK_BOX(vbox), overlay, TRUE, TRUE, 0);

    notebook = gtk_notebook_new();
    gtk_notebook_set_scrollable(GTK_NOTEBOOK(notebook), TRUE);
    gtk_container_add(GTK_CONTAINER(overlay), notebook);

    status_overlay = gtk_label_new("");
    gtk_widget_set_halign(status_overlay, GTK_ALIGN_START);
    gtk_widget_set_valign(status_overlay, GTK_ALIGN_END);
    
    GtkCssProvider* css = gtk_css_provider_new();
    gtk_css_provider_load_from_data(css, 
        "label { background-color: rgba(50, 50, 50, 0.8); color: white; padding: 4px 8px; border-radius: 4px; font-size: 12px; }", -1, NULL);
    gtk_style_context_add_provider(gtk_widget_get_style_context(status_overlay), GTK_STYLE_PROVIDER(css), GTK_STYLE_PROVIDER_PRIORITY_USER);
    
    gtk_widget_set_margin_start(status_overlay, 10);
    gtk_widget_set_margin_bottom(status_overlay, 10);
    gtk_overlay_add_overlay(GTK_OVERLAY(overlay), status_overlay);
    gtk_widget_hide(status_overlay);

    gtk_container_add(GTK_CONTAINER(window), vbox);

    g_signal_connect(url_entry, "activate", G_CALLBACK(on_url_activate), NULL);
    g_signal_connect(btn_back, "clicked", G_CALLBACK(on_back), NULL);
    g_signal_connect(btn_forward, "clicked", G_CALLBACK(on_forward), NULL);
    g_signal_connect(btn_refresh, "clicked", G_CALLBACK(on_refresh), NULL);
    g_signal_connect(btn_new_tab, "clicked", G_CALLBACK(on_new_tab), NULL);
    g_signal_connect(btn_close_tab, "clicked", G_CALLBACK(on_close_tab), NULL);
    g_signal_connect(btn_history, "clicked", G_CALLBACK(on_history), NULL);
    g_signal_connect(btn_bookmark, "clicked", G_CALLBACK(on_bookmark), NULL);
    g_signal_connect(notebook, "switch-page", G_CALLBACK(on_switch_page), NULL);
    g_signal_connect(window, "size-allocate", G_CALLBACK(on_window_size_allocate), NULL);

    add_tab("https://cn.bing.com");
}

int main(int argc, char* argv[]) {
    g_setenv("WEBKIT_DISABLE_COMPOSITING_MODE", "1", TRUE);
    g_setenv("WEBKIT_DISABLE_DMABUF_RENDERER", "1", TRUE);
    
    // 屏蔽无障碍总线警告，防止该警告引发潜在崩溃
    g_setenv("NO_AT_BRIDGE", "1", TRUE); 
    
    // 移除了 WEBKIT_GST_ENABLE_AUDIO_MIXER，这是导致音频崩溃和重播的核心原因

    gtk_init(&argc, &argv);
    GtkWidget* window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "WebBrowser");
    gtk_window_set_default_size(GTK_WINDOW(window), 800, 600);
    
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
    build_ui(window);
    gtk_widget_show_all(window);
    gtk_main();
    return 0;
}