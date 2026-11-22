#include <webkit2/webkit2.h>
#include <gtk/gtk.h>
#include <cstdio>
#include <cstring>
#include <string>
#include <sys/stat.h>
#include <ctime>
#include <vector>
#include <algorithm>
#include <sstream>
#include <cmath>
#include <cstdlib>
#include <pango/pango.h>

static void run_selection_toolbar_script(WebKitWebView* view);
void show_project_info(GtkWidget* parent);

static GtkWidget* notebook = nullptr;
static GtkWidget* url_entry = nullptr;
static GtkWidget* progress_bar = nullptr;
static GtkWidget* status_overlay = nullptr;
static guint status_hide_timer = 0;
static guint resize_timer_id = 0;
static gboolean is_typing = FALSE;
static guint32 last_webview_menu_time = 0;

static WebKitWebContext* web_context = nullptr;
static WebKitCookieManager* cookie_manager = nullptr;
static WebKitWebsiteDataManager* data_manager = nullptr;

// 独立数据目录
static std::string base_dir = "/data/data/com.termux/files/home/.config/WebBrowser/data";
static std::string history_file = base_dir + "/history/history.txt";
static std::string bookmark_file = base_dir + "/bookmarks/bookmarks.txt";
static std::string log_file = base_dir + "/logs/browser.log";

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
static GtkListStore* log_store = nullptr;
static GtkWidget* record_dialog = nullptr;
static GtkWidget* record_notebook = nullptr;
static GtkWidget* record_multi_button = nullptr;
static GtkWidget* record_select_all_button = nullptr;
static GtkWidget* record_invert_button = nullptr;
static GtkWidget* record_delete_button = nullptr;
static GtkWidget* record_copy_button = nullptr;
static bool record_multi_mode = false;
static std::vector<GtkTreeViewColumn*> record_selection_columns;
static std::vector<std::string> log_lines;
static std::string settings_file = base_dir + "/settings.conf";
static std::string ui_theme = "light";
static std::string ui_language = "zh"; // "zh", "en", "zh-TW"
static GtkCssProvider* app_css = nullptr;
static bool app_css_installed = false;
static GtkWidget* main_window = nullptr;
static GtkWidget* btn_back = nullptr;
static GtkWidget* btn_forward = nullptr;
static GtkWidget* btn_refresh = nullptr;
static GtkWidget* btn_history = nullptr;
static GtkWidget* btn_bookmark = nullptr;
static GtkWidget* btn_settings = nullptr;
static GtkWidget* tab_add_button = nullptr;
static GtkWidget* tab_close_button = nullptr;
static bool javascript_enabled = true;
static bool webgl_enabled = true;
static bool media_requires_gesture = true;
static double page_zoom_percent = 50.0;
static bool zoom_setting_version_seen = false;

// 语言翻译辅助函数支持 zh, zh-TW, en
const char* tr(const char* zh, const char* en) {
    if (ui_language == "en") return en;
    return zh;
}

const char* tr_full(const char* zh, const char* zh_tw, const char* en) {
    if (ui_language == "en") return en;
    if (ui_language == "zh-TW") return zh_tw;
    return zh;
}

GtkWidget* get_webview_from_page(GtkWidget* page) {
    if (!page || !GTK_IS_CONTAINER(page)) return nullptr;
    GList* children = gtk_container_get_children(GTK_CONTAINER(page));
    if (children) {
        GtkWidget* webview = GTK_WIDGET(children->data);
        g_list_free(children);
        return webview;
    }
    return nullptr;
}

GtkWidget* get_current_webview() {
    if (!notebook || !GTK_IS_NOTEBOOK(notebook)) return nullptr;
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
    if (status_overlay && GTK_IS_WIDGET(status_overlay))
        gtk_widget_hide(status_overlay);
    status_hide_timer = 0;
    return G_SOURCE_REMOVE;
}

void update_status_text(const char* text, gboolean auto_hide) {
    if (!status_overlay || !GTK_IS_LABEL(status_overlay)) return;
    gtk_label_set_text(GTK_LABEL(status_overlay), text ? text : "");
    gtk_widget_show(status_overlay);
    if (status_hide_timer > 0) {
        g_source_remove(status_hide_timer);
        status_hide_timer = 0;
    }
    if (auto_hide) status_hide_timer = g_timeout_add(1600, hide_status_callback, NULL);
}

void on_progress_changed(WebKitWebView* web_view, GParamSpec* pspec, gpointer user_data) {
    if (GTK_WIDGET(web_view) == get_current_webview() && progress_bar &&
        webkit_web_view_is_loading(web_view)) {
        double real_progress = webkit_web_view_get_estimated_load_progress(web_view);
        gtk_widget_show(progress_bar);
        gtk_progress_bar_set_fraction(GTK_PROGRESS_BAR(progress_bar), real_progress);
        if (status_overlay && !gtk_widget_get_visible(status_overlay))
            update_status_text(tr_full("正在加载...", "正在載入...", "Loading..."), FALSE);
    }
}

std::string url_encode(const std::string& value) {
    static const char hex[] = "0123456789ABCDEF";
    std::string encoded;
    for (unsigned char c : value) {
        if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') ||
            (c >= '0' && c <= '9') || c == '-' || c == '_' ||
            c == '.' || c == '~') {
            encoded.push_back(static_cast<char>(c));
        } else {
            encoded.push_back('%');
            encoded.push_back(hex[c >> 4]);
            encoded.push_back(hex[c & 15]);
        }
    }
    return encoded;
}

std::string resolve_input(const char* input) {
    std::string s(input ? input : "");
    if (s.empty()) return "https://cn.bing.com";
    if (s.find(" ") == std::string::npos && s.find(".") != std::string::npos) {
        if (s.find("://") == std::string::npos) return "https://" + s;
        return s;
    }
    return "https://cn.bing.com/search?q=" + url_encode(s);
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

enum RecordColumns { REC_TEXT = 0, REC_URL = 1, REC_TIME = 2, REC_SELECTED = 3, REC_INDEX = 4 };

void apply_app_theme() {
    if (!app_css) app_css = gtk_css_provider_new();
    const char* css_light =
        "window, dialog { background-color: #f4f6fa; color: #20242b; }"
        "button { background-image: none; background-color: #ffffff; color: #20242b; border: 1px solid #cbd2dc; border-radius: 8px; padding: 6px 11px; }"
        "button:hover { background-color: #e8eef8; }"
        "button:active { background-color: #dce5f2; }"
        "entry, textview, text, treeview { background-color: #ffffff; color: #20242b; }"
        "notebook { background-color: #f4f6fa; }"
        "notebook header { background-color: #e9edf4; }"
        "notebook tab { background-color: #e9edf4; color: #20242b; padding: 7px 12px; }"
        "notebook tab:checked { background-color: #ffffff; color: #20242b; }"
        "checkbutton, radiobutton, label { color: #20242b; }"
        "frame { border-color: #d5dce6; }"
        "scrolledwindow { background-color: #f4f6fa; }"
        "scale trough { background-color: #d5dce6; }";
    const char* css_dark =
        "window, dialog { background-color: #202630; color: #edf0f5; }"
        "button { background-image: none; background-color: #303946; color: #edf0f5; border: 1px solid #4b5869; border-radius: 8px; padding: 6px 11px; }"
        "button:hover { background-color: #3d4a5b; }"
        "button:active { background-color: #46566a; }"
        "entry, textview, text, treeview { background-color: #29313d; color: #edf0f5; }"
        "notebook { background-color: #202630; }"
        "notebook header { background-color: #29313d; }"
        "notebook tab { background-color: #303946; color: #edf0f5; padding: 7px 12px; }"
        "notebook tab:checked { background-color: #202630; color: #edf0f5; }"
        "checkbutton, radiobutton, label { color: #edf0f5; }"
        "frame { border-color: #465263; }"
        "scrolledwindow { background-color: #202630; }"
        "scale trough { background-color: #465263; }";
    gtk_css_provider_load_from_data(app_css, ui_theme == "dark" ? css_dark : css_light, -1, NULL);
    GdkScreen* screen = gdk_screen_get_default();
    if (screen && !app_css_installed) {
        gtk_style_context_add_provider_for_screen(screen, GTK_STYLE_PROVIDER(app_css), GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
        app_css_installed = true;
    }
    GtkSettings* settings = gtk_settings_get_default();
    if (settings) g_object_set(settings, "gtk-application-prefer-dark-theme", ui_theme == "dark", NULL);
    if (main_window) gtk_widget_queue_draw(main_window);
}

void apply_ui_language() {
    if (btn_back) gtk_widget_set_tooltip_text(btn_back, tr_full("后退", "後退", "Back"));
    if (btn_forward) gtk_widget_set_tooltip_text(btn_forward, tr_full("前进", "前進", "Forward"));
    if (btn_refresh) gtk_widget_set_tooltip_text(btn_refresh, tr_full("刷新", "重新整理", "Reload"));
    if (btn_history) gtk_widget_set_tooltip_text(btn_history, tr_full("历史记录", "歷史記錄", "History"));
    if (btn_bookmark) gtk_widget_set_tooltip_text(btn_bookmark, tr_full("收藏夹", "書籤", "Bookmarks"));
    if (btn_settings) gtk_widget_set_tooltip_text(btn_settings, tr_full("设置", "設定", "Settings"));
    if (url_entry) gtk_entry_set_placeholder_text(GTK_ENTRY(url_entry), tr_full("输入网址或搜索内容", "輸入網址或搜尋內容", "Enter a URL or search"));
    if (tab_add_button) gtk_widget_set_tooltip_text(tab_add_button, tr_full("新建标签页", "新增分頁", "New tab"));
    if (tab_close_button) gtk_widget_set_tooltip_text(tab_close_button, tr_full("关闭当前标签页", "關閉目前分頁", "Close current tab"));
    if (main_window) gtk_window_set_title(GTK_WINDOW(main_window), "WebBrowser");
    if (web_context) {
        const char* zh_languages[] = {"zh-CN", "zh", "en", NULL};
        const char* zhtw_languages[] = {"zh-TW", "zh-HK", "zh", "en", NULL};
        const char* en_languages[] = {"en", "zh-CN", "zh", NULL};
        if (ui_language == "zh-TW") {
            webkit_web_context_set_preferred_languages(web_context, zhtw_languages);
        } else {
            webkit_web_context_set_preferred_languages(web_context, ui_language == "en" ? en_languages : zh_languages);
        }
    }
}

void load_settings() {
    FILE* f = fopen(settings_file.c_str(), "r");
    if (!f) return;
    char key[128], value[256];
    while (fscanf(f, "%127[^=]=%255s\n", key, value) == 2) {
        if (strcmp(key, "theme") == 0 && (strcmp(value, "light") == 0 || strcmp(value, "dark") == 0)) ui_theme = value;
        else if (strcmp(key, "language") == 0 && (strcmp(value, "zh") == 0 || strcmp(value, "en") == 0 || strcmp(value, "zh-TW") == 0)) ui_language = value;
        else if (strcmp(key, "javascript_enabled") == 0) javascript_enabled = strcmp(value, "1") == 0;
        else if (strcmp(key, "webgl_enabled") == 0) webgl_enabled = strcmp(value, "1") == 0;
        else if (strcmp(key, "media_requires_gesture") == 0) media_requires_gesture = strcmp(value, "1") == 0;
        else if (strcmp(key, "page_zoom_percent") == 0) page_zoom_percent = std::max(0.0, std::min(100.0, atof(value)));
        else if (strcmp(key, "zoom_setting_version") == 0) zoom_setting_version_seen = (strcmp(value, "2") == 0 || strcmp(value, "3") == 0);
    }
    fclose(f);
    if (!zoom_setting_version_seen) page_zoom_percent = 50.0;
}

void save_settings() {
    FILE* f = fopen(settings_file.c_str(), "w");
    if (!f) return;
    fprintf(f, "theme=%s\nlanguage=%s\njavascript_enabled=%d\nwebgl_enabled=%d\nmedia_requires_gesture=%d\npage_zoom_percent=%.0f\nzoom_setting_version=3\n",
        ui_theme.c_str(), ui_language.c_str(), javascript_enabled ? 1 : 0,
        webgl_enabled ? 1 : 0, media_requires_gesture ? 1 : 0, page_zoom_percent);
    fclose(f);
}

void refresh_history_ui() {
    if (!history_store) return;
    gtk_list_store_clear(history_store);
    for (int i = (int)history_list.size() - 1; i >= 0; --i) {
        GtkTreeIter iter;
        gtk_list_store_append(history_store, &iter);
        gtk_list_store_set(history_store, &iter, REC_TEXT, history_list[i].title.c_str(),
            REC_URL, history_list[i].url.c_str(), REC_TIME, history_list[i].time.c_str(),
            REC_SELECTED, FALSE, REC_INDEX, i, -1);
    }
}

void refresh_bookmark_ui() {
    if (!bookmark_store) return;
    gtk_list_store_clear(bookmark_store);
    for (int i = (int)bookmark_list.size() - 1; i >= 0; --i) {
        GtkTreeIter iter;
        gtk_list_store_append(bookmark_store, &iter);
        gtk_list_store_set(bookmark_store, &iter, REC_TEXT, bookmark_list[i].title.c_str(),
            REC_URL, bookmark_list[i].url.c_str(), REC_TIME, bookmark_list[i].time.c_str(),
            REC_SELECTED, FALSE, REC_INDEX, i, -1);
    }
}

void refresh_log_ui() {
    if (!log_store) return;
    gtk_list_store_clear(log_store);
    for (int i = (int)log_lines.size() - 1; i >= 0; --i) {
        GtkTreeIter iter;
        gtk_list_store_append(log_store, &iter);
        gtk_list_store_set(log_store, &iter, 0, log_lines[i].c_str(), 1, FALSE, 2, i, -1);
    }
}

int record_selected_column(GtkTreeModel* model) {
    const int count = gtk_tree_model_get_n_columns(model);
    if (count == 3) return 1; // 日志
    if (count == 4) return 2; // Cookies
    return REC_SELECTED;      // 历史记录与收藏
}

void on_record_toggle(GtkCellRendererToggle* renderer, gchar* path_str, gpointer data) {
    GtkTreeModel* model = GTK_TREE_MODEL(data);
    GtkTreePath* path = gtk_tree_path_new_from_string(path_str);
    GtkTreeIter iter;
    if (gtk_tree_model_get_iter(model, &iter, path)) {
        gboolean selected = FALSE;
        const int selected_col = record_selected_column(model);
        gtk_tree_model_get(model, &iter, selected_col, &selected, -1);
        gtk_list_store_set(GTK_LIST_STORE(model), &iter, selected_col, !selected, -1);
    }
    gtk_tree_path_free(path);
}

gboolean close_record_dialog_idle(gpointer) {
    if (record_dialog)
        gtk_dialog_response(GTK_DIALOG(record_dialog), GTK_RESPONSE_CLOSE);
    return G_SOURCE_REMOVE;
}

gboolean on_record_button_press(GtkWidget* tree, GdkEventButton* event, gpointer) {
    if (event->type != GDK_BUTTON_PRESS || event->button != 3) return FALSE;

    GtkTreePath* path = nullptr;
    if (!gtk_tree_view_get_path_at_pos(GTK_TREE_VIEW(tree), (int)event->x, (int)event->y,
            &path, nullptr, nullptr, nullptr))
        return FALSE;

    GtkTreeModel* model = gtk_tree_view_get_model(GTK_TREE_VIEW(tree));
    GtkTreeIter iter;
    if (!gtk_tree_model_get_iter(model, &iter, path)) {
        gtk_tree_path_free(path);
        return FALSE;
    }
    gtk_tree_path_free(path);

    gchar* url = nullptr;
    gtk_tree_model_get(model, &iter, REC_URL, &url, -1);
    if (url && *url) {
        GtkWidget* wv = get_current_webview();
        if (wv) {
            webkit_web_view_load_uri(WEBKIT_WEB_VIEW(wv), url);
            update_status_text(tr_full("正在访问", "正在存取", "Opening"), TRUE);
        }
        g_idle_add(close_record_dialog_idle, nullptr);
    }
    g_free(url);
    return TRUE;
}

GtkWidget* create_record_tree(GtkListStore* store, bool log_mode) {
    GtkWidget* tree = gtk_tree_view_new_with_model(GTK_TREE_MODEL(store));

    {
        GtkCellRenderer* toggle = gtk_cell_renderer_toggle_new();
        const int selected_col = log_mode ? 1 : REC_SELECTED;
        GtkTreeViewColumn* select_col = gtk_tree_view_column_new_with_attributes(
            tr_full("选择", "選擇", "Select"), toggle, "active", selected_col, NULL);
        gtk_tree_view_append_column(GTK_TREE_VIEW(tree), select_col);
        record_selection_columns.push_back(select_col);
        g_signal_connect(toggle, "toggled", G_CALLBACK(on_record_toggle), store);
    }

    GtkCellRenderer* text = gtk_cell_renderer_text_new();
    g_object_set(text, "ellipsize", PANGO_ELLIPSIZE_END, "ellipsize-set", TRUE,
                 "width-chars", 20, "max-width-chars", 34, NULL);
    GtkTreeViewColumn* col = gtk_tree_view_column_new_with_attributes(
        log_mode ? tr_full("日志内容", "日誌內容", "Log entry") : tr_full("网页标题", "網頁標題", "Page title"),
        text, "text", 0, NULL);
    gtk_tree_view_column_set_expand(col, TRUE);
    gtk_tree_view_column_set_resizable(col, TRUE);
    gtk_tree_view_append_column(GTK_TREE_VIEW(tree), col);

    if (!log_mode) {
        text = gtk_cell_renderer_text_new();
        g_object_set(text, "ellipsize", PANGO_ELLIPSIZE_END, "ellipsize-set", TRUE,
                     "width-chars", 22, "max-width-chars", 38, NULL);
        col = gtk_tree_view_column_new_with_attributes(tr_full("网址", "網址", "Address"), text, "text", REC_URL, NULL);
        gtk_tree_view_column_set_expand(col, TRUE);
        gtk_tree_view_column_set_resizable(col, TRUE);
        gtk_tree_view_append_column(GTK_TREE_VIEW(tree), col);
        text = gtk_cell_renderer_text_new();
        g_object_set(text, "ellipsize", PANGO_ELLIPSIZE_END, "ellipsize-set", TRUE,
                     "width-chars", 17, "max-width-chars", 22, NULL);
        col = gtk_tree_view_column_new_with_attributes(tr_full("时间", "時間", "Time"), text, "text", REC_TIME, NULL);
        gtk_tree_view_column_set_resizable(col, TRUE);
        gtk_tree_view_append_column(GTK_TREE_VIEW(tree), col);
    }
    if (!log_mode)
        g_signal_connect(tree, "button-press-event",
                         G_CALLBACK(on_record_button_press), nullptr);
    return tree;
}

void set_record_multi_mode(bool enabled) {
    record_multi_mode = enabled;
    if (record_multi_button) gtk_button_set_label(GTK_BUTTON(record_multi_button), enabled ? tr_full("退出多选", "退出多選", "Exit selection") : tr_full("多选", "多選", "Select"));
    if (record_select_all_button) gtk_widget_set_visible(record_select_all_button, enabled);
    if (record_invert_button) gtk_widget_set_visible(record_invert_button, enabled);
    if (record_delete_button) {
        gtk_widget_set_visible(record_delete_button, enabled);
        const int active_page = record_notebook ? gtk_notebook_get_current_page(GTK_NOTEBOOK(record_notebook)) : -1;
        gtk_widget_set_sensitive(record_delete_button, active_page != 2);
    }
    if (record_copy_button) gtk_widget_set_visible(record_copy_button, enabled);
    for (GtkTreeViewColumn* column : record_selection_columns)
        gtk_tree_view_column_set_visible(column, enabled);
}

GtkListStore* current_record_store() {
    int page = gtk_notebook_get_current_page(GTK_NOTEBOOK(record_notebook));
    if (page == 0) return history_store;
    if (page == 1) return bookmark_store;
    if (page == 2) return cookie_store;
    if (page == 3) return log_store;
    return nullptr;
}

void on_record_multi(GtkWidget*, gpointer) { set_record_multi_mode(!record_multi_mode); }

void on_record_select_all(GtkWidget*, gpointer) {
    GtkListStore* store = current_record_store();
    if (!store) return;
    GtkTreeIter iter;
    gboolean valid = gtk_tree_model_get_iter_first(GTK_TREE_MODEL(store), &iter);
    while (valid) {
        const int selected_col = record_selected_column(GTK_TREE_MODEL(store));
        gtk_list_store_set(store, &iter, selected_col, TRUE, -1);
        valid = gtk_tree_model_iter_next(GTK_TREE_MODEL(store), &iter);
    }
}

void on_record_invert(GtkWidget*, gpointer) {
    GtkListStore* store = current_record_store();
    if (!store) return;
    GtkTreeIter iter;
    gboolean valid = gtk_tree_model_get_iter_first(GTK_TREE_MODEL(store), &iter);
    while (valid) {
        gboolean selected = FALSE;
        const int selected_col = record_selected_column(GTK_TREE_MODEL(store));
        gtk_tree_model_get(GTK_TREE_MODEL(store), &iter, selected_col, &selected, -1);
        gtk_list_store_set(GTK_LIST_STORE(store), &iter, selected_col, !selected, -1);
        valid = gtk_tree_model_iter_next(GTK_TREE_MODEL(store), &iter);
    }
}

void on_record_copy_selected(GtkWidget*, gpointer) {
    int page = gtk_notebook_get_current_page(GTK_NOTEBOOK(record_notebook));
    GtkListStore* store = current_record_store();
    if (!store) return;
    std::string copied;
    GtkTreeIter iter;
    gboolean valid = gtk_tree_model_get_iter_first(GTK_TREE_MODEL(store), &iter);
    while (valid) {
        gboolean selected = FALSE;
        gtk_tree_model_get(GTK_TREE_MODEL(store), &iter, record_selected_column(GTK_TREE_MODEL(store)), &selected, -1);
        if (selected) {
            gchar *a = nullptr, *b = nullptr, *c = nullptr;
            if (page == 0 || page == 1) {
                gtk_tree_model_get(GTK_TREE_MODEL(store), &iter, REC_TEXT, &a, REC_URL, &b, REC_TIME, &c, -1);
                copied += (a ? a : "") + std::string("\t") + (b ? b : "") + "\t" + (c ? c : "") + "\n";
            } else if (page == 2) {
                gtk_tree_model_get(GTK_TREE_MODEL(store), &iter, 0, &a, 1, &b, -1);
                copied += std::string(a ? a : "") + "\t" + (b ? b : "") + "\n";
            } else {
                gtk_tree_model_get(GTK_TREE_MODEL(store), &iter, 0, &a, -1);
                copied += std::string(a ? a : "") + "\n";
            }
            g_free(a); g_free(b); g_free(c);
        }
        valid = gtk_tree_model_iter_next(GTK_TREE_MODEL(store), &iter);
    }
    if (copied.empty()) {
        update_status_text(tr_full("请先勾选要复制的项目", "請先勾選要複製的項目", "Select items to copy first"), TRUE);
        return;
    }
    GtkClipboard* clipboard = gtk_clipboard_get(GDK_SELECTION_CLIPBOARD);
    gtk_clipboard_set_text(clipboard, copied.c_str(), (gint)copied.size());
    update_status_text(tr_full("已复制选中内容", "已複製選中內容", "Selected items copied"), TRUE);
}

void on_record_delete_selected(GtkWidget*, gpointer) {
    int page = gtk_notebook_get_current_page(GTK_NOTEBOOK(record_notebook));
    GtkListStore* store = current_record_store();
    if (!store) return;
    if (page == 2) {
        update_status_text(tr_full("Cookie 目前支持复制选中内容，删除请使用清空", "Cookie 目前支援複製選中內容，刪除請使用清空", "Cookies can be copied selectively; use Clear to remove them"), TRUE);
        return;
    }
    std::vector<int> indices;
    GtkTreeIter iter;
    gboolean valid = gtk_tree_model_get_iter_first(GTK_TREE_MODEL(store), &iter);
    while (valid) {
        gboolean selected = FALSE;
        gint index = -1;
        const bool log_mode = page == 3;
        gtk_tree_model_get(GTK_TREE_MODEL(store), &iter,
            log_mode ? 1 : REC_SELECTED, &selected,
            log_mode ? 2 : REC_INDEX, &index, -1);
        if (selected && index >= 0) indices.push_back(index);
        valid = gtk_tree_model_iter_next(GTK_TREE_MODEL(store), &iter);
    }
    std::sort(indices.rbegin(), indices.rend());
    for (int index : indices) {
        if (page == 0 && index < (int)history_list.size()) history_list.erase(history_list.begin() + index);
        else if (page == 1 && index < (int)bookmark_list.size()) bookmark_list.erase(bookmark_list.begin() + index);
        else if (page == 3 && index < (int)log_lines.size()) log_lines.erase(log_lines.begin() + index);
    }
    if (page == 0) { write_records(history_file, history_list); refresh_history_ui(); }
    else if (page == 1) { write_records(bookmark_file, bookmark_list); refresh_bookmark_ui(); }
    else if (page == 3) {
        FILE* f = fopen(log_file.c_str(), "w");
        if (f) { for (const auto& line : log_lines) fprintf(f, "%s\n", line.c_str()); fclose(f); }
        refresh_log_ui();
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
            gtk_list_store_set(cookie_store, &iter, 0, domain ? domain : "", 1, name ? name : "", 2, FALSE, 3, gtk_tree_model_iter_n_children(GTK_TREE_MODEL(cookie_store), NULL), -1);
        }
        g_list_free_full(cookies, (GDestroyNotify)soup_cookie_free);
    }
    if (error) g_error_free(error);
}

void on_history(GtkWidget* w, gpointer d) {
    read_records(history_file, history_list);
    read_records(bookmark_file, bookmark_list);
    record_multi_mode = false;
    record_selection_columns.clear();

    record_dialog = gtk_dialog_new();
    gtk_window_set_title(GTK_WINDOW(record_dialog), tr_full("浏览数据", "瀏覽資料", "Browsing Data"));
    gtk_window_set_transient_for(GTK_WINDOW(record_dialog), GTK_WINDOW(gtk_widget_get_toplevel(w)));
    gtk_window_set_modal(GTK_WINDOW(record_dialog), TRUE);
    gtk_window_set_destroy_with_parent(GTK_WINDOW(record_dialog), TRUE);
    gtk_window_set_default_size(GTK_WINDOW(record_dialog), 680, 430);

    GtkWidget* content = gtk_dialog_get_content_area(GTK_DIALOG(record_dialog));
    record_notebook = gtk_notebook_new();
    gtk_box_pack_start(GTK_BOX(content), record_notebook, TRUE, TRUE, 0);

    GtkWidget* scroll1 = gtk_scrolled_window_new(NULL, NULL);
    history_store = gtk_list_store_new(5, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_BOOLEAN, G_TYPE_INT);
    GtkWidget* tree1 = create_record_tree(history_store, false);
    gtk_container_add(GTK_CONTAINER(scroll1), tree1);
    gtk_notebook_append_page(GTK_NOTEBOOK(record_notebook), scroll1, gtk_label_new(tr_full("访问记录", "訪問記錄", "History")));
    refresh_history_ui();

    GtkWidget* scroll2 = gtk_scrolled_window_new(NULL, NULL);
    bookmark_store = gtk_list_store_new(5, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_BOOLEAN, G_TYPE_INT);
    GtkWidget* tree2 = create_record_tree(bookmark_store, false);
    gtk_container_add(GTK_CONTAINER(scroll2), tree2);
    gtk_notebook_append_page(GTK_NOTEBOOK(record_notebook), scroll2, gtk_label_new(tr_full("网页收藏", "網頁收藏", "Bookmarks")));
    refresh_bookmark_ui();

    GtkWidget* scroll3 = gtk_scrolled_window_new(NULL, NULL);
    cookie_store = gtk_list_store_new(4, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_BOOLEAN, G_TYPE_INT);
    GtkWidget* tree3 = gtk_tree_view_new_with_model(GTK_TREE_MODEL(cookie_store));
    GtkCellRenderer* cookie_toggle = gtk_cell_renderer_toggle_new();
    GtkTreeViewColumn* cookie_select_col = gtk_tree_view_column_new_with_attributes(tr_full("选择", "選擇", "Select"), cookie_toggle, "active", 2, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(tree3), cookie_select_col);
    record_selection_columns.push_back(cookie_select_col);
    g_signal_connect(cookie_toggle, "toggled", G_CALLBACK(on_record_toggle), cookie_store);
    GtkCellRenderer* cr = gtk_cell_renderer_text_new();
    g_object_set(cr, "ellipsize", PANGO_ELLIPSIZE_END, "ellipsize-set", TRUE, "width-chars", 22, "max-width-chars", 36, NULL);
    GtkTreeViewColumn* cookie_domain_col = gtk_tree_view_column_new_with_attributes(tr_full("域名", "網域", "Domain"), cr, "text", 0, NULL);
    gtk_tree_view_column_set_expand(cookie_domain_col, TRUE);
    gtk_tree_view_append_column(GTK_TREE_VIEW(tree3), cookie_domain_col);
    cr = gtk_cell_renderer_text_new();
    g_object_set(cr, "ellipsize", PANGO_ELLIPSIZE_END, "ellipsize-set", TRUE, "width-chars", 18, "max-width-chars", 30, NULL);
    GtkTreeViewColumn* cookie_name_col = gtk_tree_view_column_new_with_attributes(tr_full("名称", "名稱", "Name"), cr, "text", 1, NULL);
    gtk_tree_view_column_set_expand(cookie_name_col, TRUE);
    gtk_tree_view_append_column(GTK_TREE_VIEW(tree3), cookie_name_col);
    gtk_container_add(GTK_CONTAINER(scroll3), tree3);
    gtk_notebook_append_page(GTK_NOTEBOOK(record_notebook), scroll3, gtk_label_new("Cookies"));
    webkit_cookie_manager_get_all_cookies(cookie_manager, NULL, on_cookies_fetched, NULL);

    GtkWidget* scroll4 = gtk_scrolled_window_new(NULL, NULL);
    log_store = gtk_list_store_new(3, G_TYPE_STRING, G_TYPE_BOOLEAN, G_TYPE_INT);
    GtkWidget* log_tree = create_record_tree(log_store, true);
    gtk_container_add(GTK_CONTAINER(scroll4), log_tree);
    gtk_notebook_append_page(GTK_NOTEBOOK(record_notebook), scroll4, gtk_label_new(tr_full("日志", "日誌", "Logs")));
    log_lines.clear();
    FILE* lf = fopen(log_file.c_str(), "r");
    if (lf) {
        char buf[4096];
        while (fgets(buf, sizeof(buf), lf)) {
            std::string line(buf);
            while (!line.empty() && (line.back() == '\n' || line.back() == '\r')) line.pop_back();
            log_lines.push_back(line);
        }
        fclose(lf);
    }
    refresh_log_ui();

    GtkWidget* bottom_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 6);
    gtk_container_set_border_width(GTK_CONTAINER(bottom_box), 8);

    GtkWidget* left_toolbar = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 6);
    record_multi_button = gtk_button_new_with_label(tr_full("多选", "多選", "Select"));
    record_select_all_button = gtk_button_new_with_label(tr_full("全选", "全選", "Select all"));
    record_invert_button = gtk_button_new_with_label(tr_full("反选", "反選", "Invert"));
    record_delete_button = gtk_button_new_with_label(tr_full("删除选中", "刪除選中", "Delete selected"));
    record_copy_button = gtk_button_new_with_label(tr_full("复制选中", "複製選中", "Copy selected"));

    gtk_box_pack_start(GTK_BOX(left_toolbar), record_multi_button, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(left_toolbar), record_select_all_button, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(left_toolbar), record_invert_button, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(left_toolbar), record_delete_button, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(left_toolbar), record_copy_button, FALSE, FALSE, 0);

    GtkWidget* close_button = gtk_button_new_with_label(tr_full("关闭", "關閉", "Close"));
    g_signal_connect_swapped(close_button, "clicked", G_CALLBACK(+[](GtkWidget* dlg){
        gtk_dialog_response(GTK_DIALOG(dlg), GTK_RESPONSE_CLOSE);
    }), record_dialog);

    gtk_box_pack_start(GTK_BOX(bottom_box), left_toolbar, FALSE, FALSE, 0);
    GtkWidget* spacer = gtk_label_new("");
    gtk_widget_set_hexpand(spacer, TRUE);
    gtk_box_pack_start(GTK_BOX(bottom_box), spacer, TRUE, TRUE, 0);
    gtk_box_pack_end(GTK_BOX(bottom_box), close_button, FALSE, FALSE, 0);

    gtk_box_pack_start(GTK_BOX(content), bottom_box, FALSE, FALSE, 0);

    g_signal_connect(record_multi_button, "clicked", G_CALLBACK(on_record_multi), NULL);
    g_signal_connect(record_select_all_button, "clicked", G_CALLBACK(on_record_select_all), NULL);
    g_signal_connect(record_invert_button, "clicked", G_CALLBACK(on_record_invert), NULL);
    g_signal_connect(record_delete_button, "clicked", G_CALLBACK(on_record_delete_selected), NULL);
    g_signal_connect(record_copy_button, "clicked", G_CALLBACK(on_record_copy_selected), NULL);

    g_signal_connect(record_notebook, "switch-page", G_CALLBACK(+[](GtkNotebook*, GtkWidget*, guint page, gpointer) {
        set_record_multi_mode(false);
        const gboolean supports_selection = (page >= 0 && page <= 3);
        if (record_multi_button) gtk_widget_set_visible(record_multi_button, supports_selection);
        if (record_select_all_button) gtk_widget_set_visible(record_select_all_button, supports_selection && record_multi_mode);
        if (record_invert_button) gtk_widget_set_visible(record_invert_button, supports_selection && record_multi_mode);
        if (record_delete_button) gtk_widget_set_visible(record_delete_button, supports_selection && record_multi_mode);
        if (record_copy_button) gtk_widget_set_visible(record_copy_button, supports_selection && record_multi_mode);
    }), NULL);

    set_record_multi_mode(false);
    gtk_widget_hide(record_multi_button);
    gtk_widget_hide(record_select_all_button);
    gtk_widget_hide(record_invert_button);
    gtk_widget_hide(record_delete_button);
    gtk_widget_hide(record_copy_button);

    gtk_widget_show_all(record_dialog);
    set_record_multi_mode(false);
    gtk_dialog_run(GTK_DIALOG(record_dialog));
    gtk_widget_destroy(record_dialog);

    record_dialog = nullptr;
    record_notebook = nullptr;
    record_selection_columns.clear();
    history_store = bookmark_store = cookie_store = log_store = nullptr;
    record_multi_button = record_select_all_button = record_invert_button = record_delete_button = record_copy_button = nullptr;
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
                GtkWidget* dialog = gtk_message_dialog_new(
                    GTK_WINDOW(gtk_widget_get_toplevel(w)),
                    (GtkDialogFlags)GTK_DIALOG_MODAL,
                    GTK_MESSAGE_INFO,
                    GTK_BUTTONS_OK,
                    "%s",
                    tr_full("该页面已收藏！", "該頁面已收藏！", "This page is already bookmarked!")
                );
                gtk_dialog_run(GTK_DIALOG(dialog));
                gtk_widget_destroy(dialog);
                return;
            }
        }
        bookmark_list.push_back({time_buf, t, uri});
        write_records(bookmark_file, bookmark_list);
        GtkWidget* dialog = gtk_message_dialog_new(
            GTK_WINDOW(gtk_widget_get_toplevel(w)),
            (GtkDialogFlags)GTK_DIALOG_MODAL,
            GTK_MESSAGE_INFO,
            GTK_BUTTONS_OK,
            "%s",
            tr_full("收藏成功！", "收藏成功！", "Bookmark added successfully!")
        );
        gtk_dialog_run(GTK_DIALOG(dialog));
        gtk_widget_destroy(dialog);
    }
}

void apply_page_zoom(GtkWidget* view);

gboolean on_url_entry_focus_in(GtkWidget* widget, GdkEventFocus* event, gpointer user_data) {
    is_typing = TRUE;
    return FALSE;
}

gboolean on_url_entry_focus_out(GtkWidget* widget, GdkEventFocus* event, gpointer user_data) {
    is_typing = FALSE;
    GtkWidget* webview = get_current_webview();
    if (webview) {
        const gchar* uri = webkit_web_view_get_uri(WEBKIT_WEB_VIEW(webview));
        if (url_entry && GTK_IS_ENTRY(url_entry))
            gtk_entry_set_text(GTK_ENTRY(url_entry), uri ? uri : "");
        apply_page_zoom(webview);
    }
    return FALSE;
}

static void context_menu_run_js_finished(GObject* object, GAsyncResult* result, gpointer) {
    if (!object || !WEBKIT_IS_WEB_VIEW(object) || !result) return;
    GError* error = nullptr;
    JSCValue* js_value =
        webkit_web_view_evaluate_javascript_finish(WEBKIT_WEB_VIEW(object), result, &error);
    if (js_value) g_object_unref(js_value);
    if (error) g_error_free(error);
}

static std::string get_trimmed_entry_selection(GtkEditable* editable) {
    if (!editable || !GTK_IS_EDITABLE(editable)) return "";
    gint start_pos = 0, end_pos = 0;
    if (!gtk_editable_get_selection_bounds(editable, &start_pos, &end_pos) || start_pos == end_pos) {
        return "";
    }
    if (start_pos > end_pos) std::swap(start_pos, end_pos);
    gchar* selected = gtk_editable_get_chars(editable, start_pos, end_pos);
    std::string text(selected ? selected : "");
    g_free(selected);
    while (!text.empty() && std::isspace(static_cast<unsigned char>(text.front()))) text.erase(text.begin());
    while (!text.empty() && std::isspace(static_cast<unsigned char>(text.back()))) text.pop_back();
    return text;
}

enum class ContextActionType {
    Cut,
    Copy,
    Paste,
    Delete,
    SelectAll,
    SearchSelected
};

struct UnifiedMenuItemData {
    GObject* target;
    bool is_entry;
    ContextActionType action;
    gboolean target_alive;
};

static void on_unified_target_weak_notify(gpointer data, GObject*) {
    auto* item_data = static_cast<UnifiedMenuItemData*>(data);
    if (item_data) {
        item_data->target_alive = FALSE;
        item_data->target = nullptr;
    }
}

static void on_unified_menu_item_destroy(gpointer data, GClosure*) {
    auto* item_data = static_cast<UnifiedMenuItemData*>(data);
    if (!item_data) return;
    if (item_data->target_alive && item_data->target) {
        g_object_weak_unref(item_data->target, on_unified_target_weak_notify, item_data);
    }
    delete item_data;
}

static void on_unified_menu_item_activate(GtkMenuItem*, gpointer user_data) {
    auto* d = static_cast<UnifiedMenuItemData*>(user_data);
    if (!d || !d->target_alive || !d->target) return;

    if (d->is_entry) {
        if (!GTK_IS_ENTRY(d->target) || !GTK_IS_EDITABLE(d->target)) return;
        GtkEntry* entry = GTK_ENTRY(d->target);
        GtkEditable* editable = GTK_EDITABLE(entry);
        gint start_pos = 0, end_pos = 0;
        const gboolean has_sel = gtk_editable_get_selection_bounds(editable, &start_pos, &end_pos) && (start_pos != end_pos);

        switch (d->action) {
            case ContextActionType::Cut:
                if (has_sel) gtk_editable_cut_clipboard(editable);
                break;
            case ContextActionType::Copy:
                if (has_sel) gtk_editable_copy_clipboard(editable);
                break;
            case ContextActionType::Paste:
                gtk_editable_paste_clipboard(editable);
                break;
            case ContextActionType::Delete:
                if (has_sel) gtk_editable_delete_selection(editable);
                break;
            case ContextActionType::SelectAll:
                gtk_editable_select_region(editable, 0, -1);
                break;
            case ContextActionType::SearchSelected: {
                std::string selected_text = get_trimmed_entry_selection(editable);
                if (!selected_text.empty()) {
                    std::string target = "https://cn.bing.com/search?q=" + url_encode(selected_text);
                    GtkWidget* target_wv = get_current_webview();
                    if (target_wv && WEBKIT_IS_WEB_VIEW(target_wv)) {
                        is_typing = FALSE;
                        webkit_web_view_load_uri(WEBKIT_WEB_VIEW(target_wv), target.c_str());
                    } else {
                        add_tab(target.c_str());
                    }
                }
                break;
            }
        }
    } else {
        if (!WEBKIT_IS_WEB_VIEW(d->target)) return;
        WebKitWebView* webview = WEBKIT_WEB_VIEW(d->target);
        switch (d->action) {
            case ContextActionType::Cut:
                webkit_web_view_execute_editing_command(webview, "Cut");
                break;
            case ContextActionType::Copy:
                webkit_web_view_execute_editing_command(webview, "Copy");
                break;
            case ContextActionType::Paste:
                webkit_web_view_execute_editing_command(webview, "Paste");
                break;
            case ContextActionType::Delete:
                webkit_web_view_execute_editing_command(webview, "Delete");
                break;
            case ContextActionType::SelectAll:
                webkit_web_view_execute_editing_command(webview, "SelectAll");
                break;
            case ContextActionType::SearchSelected: {
                const char* script =
                    "(()=>{const s=String(window.getSelection()).trim();"
                    "if(s)location.href='https://cn.bing.com/search?q='+encodeURIComponent(s);})();";
                webkit_web_view_evaluate_javascript(webview, script, -1,
                    nullptr, nullptr, nullptr,
                    context_menu_run_js_finished, nullptr);
                break;
            }
        }
    }
}

// 统一构建 GTK 输入框与网页视图的右键菜单：项目完全一致、顺序与布局完全平等
static void populate_unified_context_menu(
    GtkMenuShell* menu_shell,
    GtkEntry* entry,
    WebKitWebView* webview
) {
    if (!menu_shell || !GTK_IS_MENU_SHELL(menu_shell)) return;
    const bool is_entry = (entry != nullptr && GTK_IS_ENTRY(entry));
    const bool is_webview = (webview != nullptr && WEBKIT_IS_WEB_VIEW(webview));
    if (!is_entry && !is_webview) return;

    GObject* target_obj = is_entry ? G_OBJECT(entry) : G_OBJECT(webview);

    gboolean has_selection = FALSE;
    gboolean has_text = FALSE;
    if (is_entry) {
        gint start_pos = 0, end_pos = 0;
        has_selection = gtk_editable_get_selection_bounds(GTK_EDITABLE(entry), &start_pos, &end_pos) && (start_pos != end_pos);
        const gchar* current_text = gtk_entry_get_text(entry);
        has_text = (current_text && *current_text);
    } else {
        // 网页视图在右键菜单中保留全部操作入口可用，与输入框结构对齐
        has_selection = TRUE;
        has_text = TRUE;
    }

    auto append_action_item = [&](const char* zh, const char* zhtw, const char* en,
                                   gboolean sensitive, ContextActionType action) {
        GtkWidget* item = gtk_menu_item_new_with_label(tr_full(zh, zhtw, en));
        gtk_widget_set_sensitive(item, sensitive);

        auto* item_data = new UnifiedMenuItemData{target_obj, is_entry, action, TRUE};
        g_object_weak_ref(target_obj, on_unified_target_weak_notify, item_data);
        g_signal_connect_data(item, "activate",
            G_CALLBACK(on_unified_menu_item_activate),
            item_data,
            on_unified_menu_item_destroy,
            (GConnectFlags)0);
        gtk_menu_shell_append(menu_shell, item);
    };

    // 1. 剪切
    append_action_item("剪切", "剪下", "Cut", has_selection, ContextActionType::Cut);

    // 2. 复制
    append_action_item("复制", "複製", "Copy", has_selection, ContextActionType::Copy);

    // 3. 粘贴
    append_action_item("粘贴", "貼上", "Paste", TRUE, ContextActionType::Paste);

    // 4. 删除
    append_action_item("删除", "刪除", "Delete", has_selection, ContextActionType::Delete);

    // 5. 分隔线
    gtk_menu_shell_append(menu_shell, gtk_separator_menu_item_new());

    // 6. 全选
    append_action_item("全选", "全選", "Select all", has_text, ContextActionType::SelectAll);

    // 7. 搜索选中文本
    append_action_item("搜索选中文本", "搜尋選取文字", "Search selected text", has_selection, ContextActionType::SearchSelected);
}

// 接管 GTK 输入框右键菜单，与网页菜单完全一致
void on_url_entry_populate_popup(GtkEntry* entry, GtkWidget* popup, gpointer) {
    if (!entry || !GTK_IS_ENTRY(entry) || !popup || !GTK_IS_MENU_SHELL(popup)) return;

    GList* children = gtk_container_get_children(GTK_CONTAINER(popup));
    for (GList* l = children; l != nullptr; l = l->next) {
        gtk_widget_destroy(GTK_WIDGET(l->data));
    }
    g_list_free(children);

    populate_unified_context_menu(GTK_MENU_SHELL(popup), entry, nullptr);
    gtk_widget_show_all(popup);
}

gboolean hide_progress_if_idle(gpointer) {
    GtkWidget* current = get_current_webview();
    if (progress_bar && (!current || !webkit_web_view_is_loading(WEBKIT_WEB_VIEW(current))))
        gtk_widget_hide(progress_bar);
    return G_SOURCE_REMOVE;
}

void on_load_changed(WebKitWebView* web_view, WebKitLoadEvent load_event, gpointer user_data) {
    gboolean is_foreground = (GTK_WIDGET(web_view) == get_current_webview());

    if (load_event == WEBKIT_LOAD_STARTED) {
        if (is_foreground) {
            update_status_text(tr_full("正在连接...", "正在連線...", "Connecting..."), FALSE);
            gtk_widget_show(progress_bar);
            gtk_progress_bar_set_fraction(GTK_PROGRESS_BAR(progress_bar), 0.0);
        }
    } else if (load_event == WEBKIT_LOAD_REDIRECTED) {
        if (is_foreground) update_status_text(tr_full("正在重定向...", "正在重新導向...", "Redirecting..."), FALSE);
    } else if (load_event == WEBKIT_LOAD_COMMITTED) {
        if (is_foreground) update_status_text(tr_full("等待响应...", "等待回應...", "Waiting for response..."), FALSE);
    } else if (load_event == WEBKIT_LOAD_FINISHED) {
        run_selection_toolbar_script(web_view);
        if (is_foreground) {
            gtk_progress_bar_set_fraction(GTK_PROGRESS_BAR(progress_bar), 1.0);
            g_timeout_add(450, hide_progress_if_idle, NULL);
            update_status_text(tr_full("完成", "完成", "Done"), TRUE);
        }

        const gchar* uri = webkit_web_view_get_uri(web_view);
        const gchar* title = webkit_web_view_get_title(web_view);
        save_history(uri, title);
        write_log("加载完成: " + std::string(uri ? uri : "未知"));

        if (is_foreground && !is_typing && url_entry && GTK_IS_ENTRY(url_entry)) {
            gtk_entry_set_text(GTK_ENTRY(url_entry), uri ? uri : "");
        }
    }
}

std::string html_escape(const char* value) {
    gchar* escaped = g_markup_escape_text(value ? value : "", -1);
    std::string result = escaped ? escaped : "";
    g_free(escaped);
    return result;
}

std::string error_reason(GError* error) {
    if (!error) return tr_full("未知网络错误，请稍后重试。", "未知網路錯誤，請稍後重試。", "An unknown network error occurred. Please try again.");
    if (error->domain == WEBKIT_NETWORK_ERROR) {
        switch (error->code) {
            case WEBKIT_NETWORK_ERROR_FAILED: return tr_full("无法连接到服务器。请检查网络连接、代理或服务器状态后重试。", "無法連線到伺服器。請檢查網路連線、代理或伺服器狀態後重試。", "Could not connect to the server. Check your connection, proxy, or server status and try again.");
            case WEBKIT_NETWORK_ERROR_TRANSPORT: return tr_full("与服务器通信时连接中断。请检查网络后重试。", "與伺服器通訊時連線中斷。請檢查網路後重試。", "The connection was interrupted while communicating with the server. Check your network and try again.");
            case WEBKIT_NETWORK_ERROR_FILE_DOES_NOT_EXIST: return tr_full("服务器上的请求资源不存在，请检查网址是否正确。", "伺服器上的請求資源不存在，請檢查網址是否正確。", "The requested resource does not exist on the server. Check the URL.");
            case WEBKIT_NETWORK_ERROR_UNKNOWN_PROTOCOL: return tr_full("浏览器无法识别该网址协议。请检查网址是否以 http:// 或 https:// 开头。", "瀏覽器無法識別該網址協定。請檢查網址是否以 http:// 或 https:// 開頭。", "The browser does not recognize this URL scheme. Check that it uses http:// or https://.");
            default: break;
        }
    }
    if (error->domain == WEBKIT_POLICY_ERROR)
        return tr_full("浏览器策略阻止了此次访问，可能是网址、证书或页面权限问题。", "瀏覽器策略阻止了此次存取，可能是網址、憑證或頁面權限問題。", "The browser policy blocked this navigation. The URL, certificate, or page permissions may be responsible.");
    if (error->domain == G_IO_ERROR && error->code == G_IO_ERROR_TIMED_OUT)
        return tr_full("网页加载超时。服务器没有在规定时间内响应，请稍后重试。", "網頁載入逾時。伺服器沒有在規定時間內回應，請稍後重試。", "The page load timed out. The server did not respond in time. Please try again later.");
    std::string message = error->message ? error->message : "";
    std::string lower = message;
    for (char& c : lower) c = (char)g_ascii_tolower(c);
    if (lower.find("resolve") != std::string::npos || lower.find("dns") != std::string::npos || lower.find("name or service not known") != std::string::npos)
        return tr_full("无法解析服务器地址。请检查网址拼写、DNS 或网络连接。", "無法解析伺服器位址。請檢查網址拼字、DNS 或網路連線。", "The server address could not be resolved. Check the URL, DNS, or network connection.");
    if (lower.find("refused") != std::string::npos || message.find("拒绝") != std::string::npos)
        return tr_full("服务器拒绝了连接请求。服务器可能暂时不可用，或没有开放对应端口。", "伺服器拒絕了連線請求。伺服器可能暫時不可用，或沒有開放對應連接埠。", "The server refused the connection. It may be unavailable or not accepting connections on that port.");
    if (lower.find("timed out") != std::string::npos || lower.find("timeout") != std::string::npos)
        return tr_full("网页加载超时。请检查网络连接，或稍后重新加载。", "網頁載入逾時。請檢查網路連線，或稍後重新載入。", "The page load timed out. Check your connection or reload later.");
    if (lower.find("certificate") != std::string::npos || lower.find("tls") != std::string::npos || lower.find("ssl") != std::string::npos)
        return tr_full("安全连接建立失败，可能是网站证书无效或设备时间不正确。", "安全連線建立失敗，可能是網站憑證無效或裝置時間不正確。", "A secure connection could not be established. The site certificate may be invalid or the device clock may be incorrect.");
    return tr_full("网页无法完成加载。请检查网络连接、网址是否正确，或稍后重试。", "網頁無法完成載入。請檢查網路連線、網址是否正確，或稍後重試。", "The page could not be loaded. Check your network connection and URL, or try again later.");
}

std::string error_code_name(GError* error) {
    if (!error) return "UNKNOWN";
    if (error->domain == WEBKIT_NETWORK_ERROR) {
        switch (error->code) {
            case WEBKIT_NETWORK_ERROR_FAILED: return "WEBKIT_NETWORK_ERROR_FAILED";
            case WEBKIT_NETWORK_ERROR_TRANSPORT: return "WEBKIT_NETWORK_ERROR_TRANSPORT";
            case WEBKIT_NETWORK_ERROR_FILE_DOES_NOT_EXIST: return "WEBKIT_NETWORK_ERROR_FILE_DOES_NOT_EXIST";
            case WEBKIT_NETWORK_ERROR_UNKNOWN_PROTOCOL: return "WEBKIT_NETWORK_ERROR_UNKNOWN_PROTOCOL";
            case WEBKIT_NETWORK_ERROR_CANCELLED: return "WEBKIT_NETWORK_ERROR_CANCELLED";
            default: return "WEBKIT_NETWORK_ERROR";
        }
    }
    if (error->domain == WEBKIT_POLICY_ERROR) return "WEBKIT_POLICY_ERROR";
    if (error->domain == G_IO_ERROR) return "G_IO_ERROR";
    return g_quark_to_string(error->domain) ? g_quark_to_string(error->domain) : "UNKNOWN_ERROR_DOMAIN";
}

gboolean on_load_failed(WebKitWebView* web_view, WebKitLoadEvent load_event, const gchar* failing_uri, GError* error, gpointer user_data) {
    if (error && g_error_matches(error, WEBKIT_PLUGIN_ERROR, WEBKIT_PLUGIN_ERROR_WILL_HANDLE_LOAD)) return TRUE;
    if (error && g_error_matches(error, WEBKIT_PLUGIN_ERROR, WEBKIT_PLUGIN_ERROR_CANNOT_FIND_PLUGIN)) {
        write_log("媒体插件缺失: " + std::string(error->message));
        return TRUE;
    }
    if (error && (g_error_matches(error, WEBKIT_NETWORK_ERROR, WEBKIT_NETWORK_ERROR_CANCELLED) ||
                  g_error_matches(error, G_IO_ERROR, G_IO_ERROR_CANCELLED))) return TRUE;
    if (error && error->message && (strstr(error->message, "cancelled") || strstr(error->message, "Cancelled"))) return TRUE;

    const gchar* uri = failing_uri ? failing_uri : "";
    const std::string reason = error_reason(error);
    const std::string escaped_reason = html_escape(reason.c_str());
    const std::string escaped_uri = html_escape(uri);
    const std::string escaped_detail = html_escape(error && error->message ? error->message : "Unknown error");
    const std::string code_text = error_code_name(error) + (error ? " (" + std::to_string(error->code) + ")" : "");
    const std::string escaped_code = html_escape(code_text.c_str());
    const bool dark = ui_theme == "dark";
    const std::string bg = dark ? "#202630" : "#f5f7fb";
    const std::string fg = dark ? "#edf0f5" : "#20242b";
    const std::string muted = dark ? "#c4cbd6" : "#596273";
    const std::string panel = dark ? "#303946" : "#e9edf4";
    std::string html =
        "<!doctype html><html><head><meta charset='utf-8'><meta name='viewport' content='width=device-width,initial-scale=1'>"
        "<title>" + std::string(tr_full("网页无法访问", "網頁無法存取", "This site can’t be reached")) + "</title><style>"
        "*{box-sizing:border-box}html,body{min-height:100%;margin:0;font-family:Arial,'Noto Sans CJK SC',sans-serif;background:" + bg + ";color:" + fg + ";}"
        "body{padding:clamp(18px,5vw,42px)}.wrap{width:100%;max-width:780px;margin:clamp(12px,7vh,64px) auto 32px;}"
        ".x{color:#e53935;font-size:64px;font-weight:700;line-height:1;margin:0 0 18px 0;}"
        "h1{font-size:clamp(24px,4vw,32px);line-height:1.25;margin:0 0 14px;font-weight:650;}"
        "p{font-size:16px;line-height:1.75;color:" + muted + ";margin:0 0 14px;overflow-wrap:anywhere;word-break:break-word;}"
        ".url{display:block;margin:18px 0;padding:12px 14px;border-radius:9px;background:" + panel + ";overflow-wrap:anywhere;word-break:break-all;}"
        "a{color:#4285f4;text-decoration:none}a:hover{text-decoration:underline}"
        ".details{margin-top:22px;padding:16px;border-radius:10px;background:" + panel + ";overflow-wrap:anywhere;word-break:break-word;}"
        ".detail-title{font-weight:600;margin-bottom:8px;color:" + fg + ";}.code{font-family:monospace;font-size:13px;line-height:1.6;}"
        ".raw{margin-top:8px;color:" + muted + ";white-space:pre-wrap;}"
        "@media(max-width:480px){body{padding:18px}.wrap{margin:18px auto}.x{font-size:54px}p{font-size:15px}}"
        "</style></head><body><main class='wrap'>"
        "<div class='x'>×</div><h1>" + std::string(tr_full("网页无法访问", "網頁無法存取", "This site can’t be reached")) + "</h1>"
        "<p>" + escaped_reason + "</p>"
        "<div class='url'><a target='_blank' rel='noopener' href='" + escaped_uri + "'>" + escaped_uri + "</a></div>"
        "<section class='details'><div class='detail-title'>" + std::string(tr_full("错误详情", "錯誤詳情", "Error details")) + "</div>"
        "<div class='code'>" + std::string(tr_full("错误码：", "錯誤碼：", "Error code: ")) + escaped_code + "</div>"
        "<div class='raw'>" + escaped_detail + "</div></section></main></body></html>";

    write_log("加载失败: " + std::string(uri ? uri : "未知网址") + " 错误: " + (error && error->message ? error->message : "unknown"));
    if (GTK_WIDGET(web_view) == get_current_webview()) {
        gtk_widget_hide(progress_bar);
        update_status_text(tr_full("网页无法访问", "網頁無法存取", "Page unavailable"), FALSE);
    }
    webkit_web_view_load_alternate_html(web_view, html.c_str(), uri, NULL);
    return TRUE;
}

void on_web_process_terminated(WebKitWebView* web_view, WebKitWebProcessTerminationReason reason, gpointer user_data) {
    const gchar* uri = webkit_web_view_get_uri(web_view);
    if (uri) webkit_web_view_load_uri(web_view, uri);
}

void on_url_activate(GtkWidget* widget, gpointer data) {
    if (!url_entry || !GTK_IS_ENTRY(url_entry)) return;
    const char* input = gtk_entry_get_text(GTK_ENTRY(url_entry));
    std::string target = resolve_input(input);
    write_log("用户输入导航: " + target);
    is_typing = FALSE;
    GtkWidget* webview = get_current_webview();
    if (webview) webkit_web_view_load_uri(WEBKIT_WEB_VIEW(webview), target.c_str());
}

void apply_page_zoom(GtkWidget* view) {
    if (!view || !WEBKIT_IS_WEB_VIEW(view)) return;
    const gint width = gtk_widget_get_allocated_width(view);
    if (width <= 0) return;
    double zoom = (double)width / 1366.0;
    if (zoom < 0.3) zoom = 0.3;
    if (zoom > 1.0) zoom = 1.0;
    zoom *= page_zoom_percent / 100.0;
    if (zoom < 0.15) zoom = 0.15;
    if (zoom > 1.0) zoom = 1.0;
    const double current = webkit_web_view_get_zoom_level(WEBKIT_WEB_VIEW(view));
    if (std::fabs(current - zoom) > 0.01)
        webkit_web_view_set_zoom_level(WEBKIT_WEB_VIEW(view), zoom);
}

gboolean delayed_webview_zoom(gpointer) {
    resize_timer_id = 0;
    GtkWidget* view = get_current_webview();
    if (view) apply_page_zoom(view);
    return G_SOURCE_REMOVE;
}

void on_window_size_allocate(GtkWidget*, GtkAllocation*, gpointer) {
    if (resize_timer_id > 0) {
        g_source_remove(resize_timer_id);
        resize_timer_id = 0;
    }
    resize_timer_id = g_timeout_add(150, delayed_webview_zoom, NULL);
}

void apply_zoom_to_all_tabs() {
    if (!notebook || !GTK_IS_NOTEBOOK(notebook)) return;
    for (int i = 0; i < gtk_notebook_get_n_pages(GTK_NOTEBOOK(notebook)); ++i) {
        GtkWidget* page = gtk_notebook_get_nth_page(GTK_NOTEBOOK(notebook), i);
        GtkWidget* view = get_webview_from_page(page);
        if (view) apply_page_zoom(view);
    }
}

static void run_selection_toolbar_script(WebKitWebView* view) {
    if (!view || !WEBKIT_IS_WEB_VIEW(view)) return;
    const char* copy_label = ui_language == "en" ? "Copy" :
        (ui_language == "zh-TW" ? "複製" : "复制");
    const char* search_label = ui_language == "en" ? "Search" :
        (ui_language == "zh-TW" ? "搜尋" : "搜索");
    const char* select_label = ui_language == "en" ? "Select all" :
        (ui_language == "zh-TW" ? "全選" : "全选");

    std::string js = R"JS((() => {
      if (window.__wbSelectionToolbarInstalled) return;
      window.__wbSelectionToolbarInstalled = true;
      const selectable = document.createElement('style');
      selectable.textContent = '* { -webkit-user-select: text !important; user-select: text !important; }';
      (document.head || document.documentElement).appendChild(selectable);
      const labels = {copy:')JS' + std::string(copy_label) + R"JS(',search:')JS' +
          std::string(search_label) + R"JS(',all:')JS' + std::string(select_label) + R"JS('};
      const bar = document.createElement('div');
      bar.id = '__wb_selection_toolbar';
      bar.style.cssText = 'display:none;position:fixed;z-index:2147483647;padding:5px;gap:4px;' +
        'background:#252b35;color:white;border:1px solid #596575;border-radius:9px;' +
        'box-shadow:0 3px 14px #0008;font:14px sans-serif;';
      function button(text, fn) {
        const b=document.createElement('button'); b.textContent=text;
        b.style.cssText='border:0;border-radius:6px;padding:7px 10px;background:#3b4655;color:white;font:inherit;';
        b.addEventListener('mousedown', e=>e.preventDefault());
        b.addEventListener('click', fn); bar.appendChild(b);
      }
      button(labels.copy, () => {
        const text=String(window.getSelection());
        if (!text) return;
        const done=document.execCommand('copy');
        if (!done && navigator.clipboard && navigator.clipboard.writeText)
          navigator.clipboard.writeText(text).catch(()=>{});
        bar.style.display='none';
      });
      button(labels.search, () => {
        const text=String(window.getSelection()).trim();
        if (text) location.href='https://cn.bing.com/search?q='+encodeURIComponent(text);
      });
      button(labels.all, () => { document.execCommand('selectAll'); });
      (document.body || document.documentElement).appendChild(bar);
      function update() {
        const sel=window.getSelection();
        if (!sel || sel.isCollapsed || !String(sel).trim()) { bar.style.display='none'; return; }
        const r=sel.getRangeAt(0).getBoundingClientRect();
        if (!r || (!r.width && !r.height)) { bar.style.display='none'; return; }
        bar.style.display='flex';
        const x=Math.max(4,Math.min(window.innerWidth-bar.offsetWidth-4,r.left+(r.width-bar.offsetWidth)/2));
        const y=Math.max(4,r.top-bar.offsetHeight-9);
        bar.style.left=x+'px'; bar.style.top=y+'px';
      }
      document.addEventListener('mouseup',()=>setTimeout(update,30),true);
      document.addEventListener('keyup',update,true);
      document.addEventListener('touchend',()=>setTimeout(update,80),true);
      document.addEventListener('scroll',()=>{ if (bar.style.display!=='none') update(); },true);
      document.addEventListener('mousedown',e=>{if (!bar.contains(e.target)) setTimeout(()=> {
        if (!String(window.getSelection()).trim()) bar.style.display='none';
      },100);},true);
    })();)JS";
    webkit_web_view_evaluate_javascript(view, js.c_str(), -1,
        nullptr, nullptr, nullptr,
        context_menu_run_js_finished, nullptr);
}

static void show_custom_webview_menu(WebKitWebView* view, const GdkEvent* trigger_event) {
    if (!view || !WEBKIT_IS_WEB_VIEW(view)) return;
    GtkWidget* menu = gtk_menu_new();
    populate_unified_context_menu(GTK_MENU_SHELL(menu), nullptr, view);
    gtk_widget_show_all(menu);
    gtk_menu_popup_at_pointer(GTK_MENU(menu), trigger_event);
}

gboolean on_webview_context_menu(WebKitWebView* view, WebKitContextMenu*,
        GdkEvent* event, WebKitHitTestResult*, gpointer) {
    if (!view || !WEBKIT_IS_WEB_VIEW(view)) return TRUE;
    last_webview_menu_time = event ? gdk_event_get_time(event) : gtk_get_current_event_time();
    show_custom_webview_menu(view, event);
    return TRUE;
}

// 兜底捕获网页内鼠标右键按下事件，防止部分 X11 环境下 WebKit 未发出 context-menu 信号
gboolean on_webview_button_press(GtkWidget* widget, GdkEventButton* event, gpointer) {
    if (!widget || !WEBKIT_IS_WEB_VIEW(widget)) return FALSE;
    if (event && event->type == GDK_BUTTON_PRESS && event->button == 3) {
        if (event->time != 0 && event->time == last_webview_menu_time) {
            return TRUE;
        }
        last_webview_menu_time = event->time;
        gtk_widget_grab_focus(widget);
        show_custom_webview_menu(WEBKIT_WEB_VIEW(widget), reinterpret_cast<const GdkEvent*>(event));
        return TRUE;
    }
    return FALSE;
}

void add_tab(const char* url) {
    if (!notebook || !GTK_IS_NOTEBOOK(notebook) || !web_context) return;
    GtkWidget* webview = webkit_web_view_new_with_context(web_context);
    gtk_widget_set_hexpand(webview, TRUE);
    gtk_widget_set_vexpand(webview, TRUE);
    gtk_widget_set_can_focus(webview, TRUE);
    gtk_widget_add_events(webview, GDK_BUTTON_PRESS_MASK);

    WebKitSettings* settings = webkit_web_view_get_settings(WEBKIT_WEB_VIEW(webview));
    webkit_settings_set_media_playback_requires_user_gesture(settings, media_requires_gesture);
    webkit_settings_set_enable_javascript(settings, javascript_enabled);
    webkit_settings_set_enable_webgl(settings, webgl_enabled);
    webkit_settings_set_enable_write_console_messages_to_stdout(settings, FALSE);

    g_signal_connect(webview, "load-changed", G_CALLBACK(on_load_changed), NULL);
    g_signal_connect(webview, "load-failed", G_CALLBACK(on_load_failed), NULL);
    g_signal_connect(webview, "web-process-terminated", G_CALLBACK(on_web_process_terminated), NULL);
    g_signal_connect(webview, "notify::estimated-load-progress", G_CALLBACK(on_progress_changed), NULL);
    g_signal_connect(webview, "decide-policy", G_CALLBACK(on_decide_policy), NULL);
    g_signal_connect(webview, "context-menu", G_CALLBACK(on_webview_context_menu), NULL);
    g_signal_connect(webview, "button-press-event", G_CALLBACK(on_webview_button_press), NULL);

    g_signal_connect(webview, "notify::title", G_CALLBACK(+[](GtkWidget* w, GParamSpec*, gpointer){
        if (!notebook || !GTK_IS_NOTEBOOK(notebook) || !w || !WEBKIT_IS_WEB_VIEW(w)) return;
        const gchar* title = webkit_web_view_get_title(WEBKIT_WEB_VIEW(w));
        GtkWidget* page_widget = gtk_widget_get_parent(w);
        gint page = (page_widget && GTK_IS_WIDGET(page_widget)) ? gtk_notebook_page_num(GTK_NOTEBOOK(notebook), page_widget) : -1;
        if (page >= 0) gtk_notebook_set_tab_label_text(GTK_NOTEBOOK(notebook), page_widget,
            (title && *title) ? title : tr_full("新标签页", "新分頁", "New tab"));
    }), NULL);

    GtkWidget* vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_box_pack_start(GTK_BOX(vbox), webview, TRUE, TRUE, 0);
    gtk_widget_show_all(vbox);

    int page = gtk_notebook_append_page(GTK_NOTEBOOK(notebook), vbox, gtk_label_new(tr_full("新标签页", "新分頁", "New tab")));
    gtk_widget_show_all(vbox);
    gtk_widget_show_all(gtk_notebook_get_nth_page(GTK_NOTEBOOK(notebook), page));
    gtk_notebook_set_current_page(GTK_NOTEBOOK(notebook), page);
    webkit_web_view_load_uri(WEBKIT_WEB_VIEW(webview), url ? url : "https://cn.bing.com");
}

void on_switch_page(GtkNotebook* nb, GtkWidget* page, guint page_num, gpointer data) {
    is_typing = FALSE;
    GtkWidget* webview = get_webview_from_page(page);
    if (webview) {
        const gchar* uri = webkit_web_view_get_uri(WEBKIT_WEB_VIEW(webview));
        if (url_entry && GTK_IS_ENTRY(url_entry))
            gtk_entry_set_text(GTK_ENTRY(url_entry), uri ? uri : "");
        apply_page_zoom(webview);

        if (webkit_web_view_is_loading(WEBKIT_WEB_VIEW(webview))) {
            gtk_widget_show(progress_bar);
            gtk_progress_bar_set_fraction(GTK_PROGRESS_BAR(progress_bar),
                webkit_web_view_get_estimated_load_progress(WEBKIT_WEB_VIEW(webview)));
            update_status_text(tr_full("加载中...", "載入中...", "Loading..."), FALSE);
        } else {
            gtk_widget_hide(progress_bar);
            update_status_text(tr_full("完成", "完成", "Done"), TRUE);
        }
    }
}

void on_back(GtkWidget* w, gpointer d) { GtkWidget* wv = get_current_webview(); if (wv) webkit_web_view_go_back(WEBKIT_WEB_VIEW(wv)); }
void on_forward(GtkWidget* w, gpointer d) { GtkWidget* wv = get_current_webview(); if (wv) webkit_web_view_go_forward(WEBKIT_WEB_VIEW(wv)); }
void on_refresh(GtkWidget* w, gpointer d) { GtkWidget* wv = get_current_webview(); if (wv) webkit_web_view_reload(WEBKIT_WEB_VIEW(wv)); }
void on_new_tab(GtkWidget* w, gpointer d) { add_tab("https://cn.bing.com"); }

void on_close_tab(GtkWidget* w, gpointer d) {
    if (!notebook || !GTK_IS_NOTEBOOK(notebook)) return;
    int page_idx = gtk_notebook_get_current_page(GTK_NOTEBOOK(notebook));
    if (page_idx == -1) return;

    gtk_notebook_remove_page(GTK_NOTEBOOK(notebook), page_idx);
    if (gtk_notebook_get_n_pages(GTK_NOTEBOOK(notebook)) == 0) {
        add_tab("https://cn.bing.com");
    }
}

void show_project_info(GtkWidget* parent) {
    GtkWidget* dialog = gtk_dialog_new_with_buttons("About WebBrowser", GTK_WINDOW(parent),
        (GtkDialogFlags)(GTK_DIALOG_MODAL | GTK_DIALOG_DESTROY_WITH_PARENT),
        tr_full("关闭", "關閉", "Close"), GTK_RESPONSE_CLOSE, NULL);
    gtk_window_set_default_size(GTK_WINDOW(dialog), 560, 420);
    GtkWidget* area = gtk_dialog_get_content_area(GTK_DIALOG(dialog));
    GtkWidget* scroll = gtk_scrolled_window_new(NULL, NULL);
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scroll), GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
    GtkWidget* label = gtk_label_new(NULL);
    gtk_label_set_xalign(GTK_LABEL(label), 0.0f);
    gtk_label_set_yalign(GTK_LABEL(label), 0.0f);
    gtk_label_set_line_wrap(GTK_LABEL(label), TRUE);
    gtk_label_set_selectable(GTK_LABEL(label), TRUE);
    const char* markup =
        "<b>WebBrowser</b>\n\n"
        "由 Sakuragi Group 开发的轻量级 Web 浏览器，面向 Termux:X11 环境。\n\n"
        "A lightweight web browser developed by Sakuragi Group for the Termux:X11 environment.\n\n"
        "制作团队 / Developer: Sakuragi Group\n"
        "项目仓库 / Repository: <a href='https://github.com/YamaiSakuragi/WebBrowser'>https://github.com/YamaiSakuragi/WebBrowser</a>\n"
        "Telegram: <a href='https://t.me/YamaiSakuragi/'>https://t.me/YamaiSakuragi/</a>";
    gtk_label_set_markup(GTK_LABEL(label), markup);
    g_signal_connect(label, "activate-link", G_CALLBACK(+[](GtkLabel*, const gchar* uri, gpointer) -> gboolean {
        if (uri && *uri) add_tab(uri);
        return TRUE;
    }), NULL);
    gtk_container_set_border_width(GTK_CONTAINER(scroll), 16);
    gtk_container_add(GTK_CONTAINER(scroll), label);
    gtk_box_pack_start(GTK_BOX(area), scroll, TRUE, TRUE, 0);
    gtk_widget_show_all(dialog);
    gtk_dialog_run(GTK_DIALOG(dialog));
    gtk_widget_destroy(dialog);
}

void on_settings(GtkWidget* w, gpointer) {
    GtkWidget* parent = gtk_widget_get_toplevel(w);
    GtkWidget* dialog = gtk_dialog_new_with_buttons("Settings", GTK_WINDOW(parent),
        (GtkDialogFlags)(GTK_DIALOG_MODAL | GTK_DIALOG_DESTROY_WITH_PARENT),
        tr_full("保存", "儲存", "Save"), GTK_RESPONSE_ACCEPT,
        tr_full("取消", "取消", "Cancel"), GTK_RESPONSE_CANCEL, NULL);
    gtk_window_set_default_size(GTK_WINDOW(dialog), 420, 560);
    gtk_window_set_resizable(GTK_WINDOW(dialog), TRUE);
    GtkWidget* area = gtk_dialog_get_content_area(GTK_DIALOG(dialog));
    GtkWidget* scroll = gtk_scrolled_window_new(NULL, NULL);
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scroll), GTK_POLICY_NEVER, GTK_POLICY_AUTOMATIC);
    GtkWidget* box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 12);
    gtk_container_set_border_width(GTK_CONTAINER(box), 16);
    gtk_container_add(GTK_CONTAINER(scroll), box);
    gtk_box_pack_start(GTK_BOX(area), scroll, TRUE, TRUE, 0);

    GtkWidget* appearance_frame = gtk_frame_new(tr_full("外观与浏览", "外觀與瀏覽", "Appearance and browsing"));
    GtkWidget* appearance = gtk_grid_new();
    gtk_grid_set_row_spacing(GTK_GRID(appearance), 9);
    gtk_grid_set_column_spacing(GTK_GRID(appearance), 12);
    gtk_container_set_border_width(GTK_CONTAINER(appearance), 12);
    gtk_container_add(GTK_CONTAINER(appearance_frame), appearance);

    GtkWidget* theme_label = gtk_label_new(tr_full("颜色模式", "顏色模式", "Color mode"));
    gtk_widget_set_halign(theme_label, GTK_ALIGN_START);
    GtkWidget* theme_combo = gtk_combo_box_text_new();
    gtk_combo_box_text_append(GTK_COMBO_BOX_TEXT(theme_combo), "light", tr_full("浅色", "淺色", "Light"));
    gtk_combo_box_text_append(GTK_COMBO_BOX_TEXT(theme_combo), "dark", tr_full("深色", "深色", "Dark"));
    gtk_combo_box_set_active_id(GTK_COMBO_BOX(theme_combo), ui_theme.c_str());
    gtk_grid_attach(GTK_GRID(appearance), theme_label, 0, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(appearance), theme_combo, 1, 0, 1, 1);

    GtkWidget* lang_label = gtk_label_new(tr_full("界面语言", "介面語言", "Language"));
    gtk_widget_set_halign(lang_label, GTK_ALIGN_START);
    GtkWidget* lang_combo = gtk_combo_box_text_new();
    gtk_combo_box_text_append(GTK_COMBO_BOX_TEXT(lang_combo), "zh", "简体中文");
    gtk_combo_box_text_append(GTK_COMBO_BOX_TEXT(lang_combo), "zh-TW", "繁體中文");
    gtk_combo_box_text_append(GTK_COMBO_BOX_TEXT(lang_combo), "en", "English");
    gtk_combo_box_set_active_id(GTK_COMBO_BOX(lang_combo), ui_language.c_str());
    gtk_grid_attach(GTK_GRID(appearance), lang_label, 0, 1, 1, 1);
    gtk_grid_attach(GTK_GRID(appearance), lang_combo, 1, 1, 1, 1);

    GtkWidget* zoom_label = gtk_label_new(tr_full("网页缩放", "網頁縮放", "Page zoom"));
    gtk_widget_set_halign(zoom_label, GTK_ALIGN_START);
    GtkWidget* zoom_scale = gtk_scale_new_with_range(GTK_ORIENTATION_HORIZONTAL, 0.0, 100.0, 1.0);
    gtk_range_set_value(GTK_RANGE(zoom_scale), page_zoom_percent);
    gtk_scale_set_digits(GTK_SCALE(zoom_scale), 0);
    gtk_scale_set_value_pos(GTK_SCALE(zoom_scale), GTK_POS_RIGHT);
    gtk_scale_add_mark(GTK_SCALE(zoom_scale), 0.0, GTK_POS_BOTTOM, "0%");
    gtk_scale_add_mark(GTK_SCALE(zoom_scale), 50.0, GTK_POS_BOTTOM, "50%");
    gtk_scale_add_mark(GTK_SCALE(zoom_scale), 100.0, GTK_POS_BOTTOM, "100%");
    gtk_widget_set_hexpand(zoom_scale, TRUE);
    GtkWidget* zoom_note = gtk_label_new(tr_full("范围 0%–100%，默认 50%；按原版自适应比例计算。", "範圍 0%–100%，預設 50%；按原版自適應比例計算。", "Range 0%–100%, default 50%; based on the original adaptive ratio."));
    gtk_label_set_line_wrap(GTK_LABEL(zoom_note), TRUE);
    gtk_widget_set_halign(zoom_note, GTK_ALIGN_START);
    gtk_grid_attach(GTK_GRID(appearance), zoom_label, 0, 2, 1, 1);
    gtk_grid_attach(GTK_GRID(appearance), zoom_scale, 1, 2, 1, 1);
    gtk_grid_attach(GTK_GRID(appearance), zoom_note, 0, 3, 2, 1);
    gtk_box_pack_start(GTK_BOX(box), appearance_frame, FALSE, FALSE, 0);

    GtkWidget* debug_frame = gtk_frame_new(tr_full("开发者设置", "開發者設定", "Developer Settings"));
    GtkWidget* debug_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 8);
    gtk_container_set_border_width(GTK_CONTAINER(debug_box), 12);
    gtk_container_add(GTK_CONTAINER(debug_frame), debug_box);
    GtkWidget* js_check = gtk_check_button_new_with_label(tr_full("启用 JavaScript", "啟用 JavaScript", "Enable JavaScript"));
    GtkWidget* webgl_check = gtk_check_button_new_with_label(tr_full("启用 WebGL", "啟用 WebGL", "Enable WebGL"));
    GtkWidget* media_check = gtk_check_button_new_with_label(tr_full("媒体播放需要用户操作", "媒體播放需要使用者操作", "Require user gesture for media playback"));
    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(js_check), javascript_enabled);
    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(webgl_check), webgl_enabled);
    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(media_check), media_requires_gesture);
    gtk_box_pack_start(GTK_BOX(debug_box), js_check, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(debug_box), webgl_check, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(debug_box), media_check, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(box), debug_frame, FALSE, FALSE, 0);

    GtkWidget* about_frame = gtk_frame_new(tr_full("关于", "關於", "About"));
    GtkWidget* about_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 7);
    gtk_container_set_border_width(GTK_CONTAINER(about_box), 12);
    gtk_container_add(GTK_CONTAINER(about_frame), about_box);
    GtkWidget* team = gtk_label_new(tr_full("制作团队：Sakuragi Group", "製作團隊：Sakuragi Group", "Developed by Sakuragi Group"));
    gtk_widget_set_halign(team, GTK_ALIGN_START);
    gtk_box_pack_start(GTK_BOX(about_box), team, FALSE, FALSE, 0);
    GtkWidget* copyright = gtk_label_new("Copyright © Sakuragi Group");
    gtk_widget_set_halign(copyright, GTK_ALIGN_START);
    gtk_box_pack_start(GTK_BOX(about_box), copyright, FALSE, FALSE, 0);
    GtkWidget* info_button = gtk_button_new_with_label(tr_full("项目说明", "專案說明", "Project information"));
    GtkWidget* telegram_button = gtk_button_new_with_label(tr_full("Telegram 频道", "Telegram 頻道", "Telegram channel"));
    gtk_widget_set_halign(info_button, GTK_ALIGN_FILL);
    gtk_widget_set_halign(telegram_button, GTK_ALIGN_FILL);
    gtk_box_pack_start(GTK_BOX(about_box), info_button, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(about_box), telegram_button, FALSE, FALSE, 0);
    g_signal_connect(info_button, "clicked", G_CALLBACK(+[](GtkWidget* button, gpointer) {
        show_project_info(gtk_widget_get_toplevel(button));
    }), NULL);
    g_signal_connect(telegram_button, "clicked", G_CALLBACK(+[](GtkWidget*, gpointer) {
        add_tab("https://t.me/YamaiSakuragi/");
    }), NULL);
    gtk_box_pack_start(GTK_BOX(box), about_frame, FALSE, FALSE, 0);

    gtk_widget_show_all(dialog);
    const std::string old_language = ui_language;
    const gint response = gtk_dialog_run(GTK_DIALOG(dialog));
    if (response == GTK_RESPONSE_ACCEPT) {
        const gchar* selected_theme = gtk_combo_box_get_active_id(GTK_COMBO_BOX(theme_combo));
        const gchar* selected_lang = gtk_combo_box_get_active_id(GTK_COMBO_BOX(lang_combo));
        if (selected_theme) ui_theme = selected_theme;
        if (selected_lang) ui_language = selected_lang;
        page_zoom_percent = gtk_range_get_value(GTK_RANGE(zoom_scale));
        javascript_enabled = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(js_check));
        webgl_enabled = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(webgl_check));
        media_requires_gesture = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(media_check));
        save_settings();
        apply_app_theme();
        apply_ui_language();
        const bool reload_for_page_preferences = (old_language != ui_language);
        for (int i = 0; i < gtk_notebook_get_n_pages(GTK_NOTEBOOK(notebook)); ++i) {
            GtkWidget* page = gtk_notebook_get_nth_page(GTK_NOTEBOOK(notebook), i);
            GtkWidget* view = get_webview_from_page(page);
            if (view) {
                WebKitSettings* settings = webkit_web_view_get_settings(WEBKIT_WEB_VIEW(view));
                webkit_settings_set_enable_javascript(settings, javascript_enabled);
                webkit_settings_set_enable_webgl(settings, webgl_enabled);
                webkit_settings_set_media_playback_requires_user_gesture(settings, media_requires_gesture);
                if (reload_for_page_preferences) webkit_web_view_reload(WEBKIT_WEB_VIEW(view));
            }
        }
        apply_zoom_to_all_tabs();
    }
    gtk_widget_destroy(dialog);
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

    load_settings();
    apply_app_theme();
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

    main_window = window;
    GtkWidget* toolbar = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
    gtk_container_set_border_width(GTK_CONTAINER(toolbar), 5);

    btn_back = gtk_button_new_with_label("←");
    btn_forward = gtk_button_new_with_label("→");
    btn_refresh = gtk_button_new_with_label("⟳");
    btn_history = gtk_button_new_with_label("◷");
    url_entry = gtk_entry_new();
    gtk_widget_set_hexpand(url_entry, TRUE);
    btn_bookmark = gtk_button_new_with_label("☆");
    btn_settings = gtk_button_new_with_label("⚙");

    g_signal_connect(url_entry, "focus-in-event", G_CALLBACK(on_url_entry_focus_in), NULL);
    g_signal_connect(url_entry, "focus-out-event", G_CALLBACK(on_url_entry_focus_out), NULL);
    g_signal_connect(url_entry, "populate-popup", G_CALLBACK(on_url_entry_populate_popup), NULL);

    gtk_box_pack_start(GTK_BOX(toolbar), btn_back, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(toolbar), btn_forward, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(toolbar), btn_refresh, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(toolbar), btn_history, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(toolbar), url_entry, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(toolbar), btn_bookmark, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(toolbar), btn_settings, FALSE, FALSE, 0);
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

    GtkWidget* tab_actions = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 3);
    tab_add_button = gtk_button_new_with_label("+");
    tab_close_button = gtk_button_new_with_label("×");
    gtk_widget_set_tooltip_text(tab_add_button, tr_full("新建标签页", "新增分頁", "New tab"));
    gtk_widget_set_tooltip_text(tab_close_button, tr_full("关闭当前标签页", "關閉目前分頁", "Close current tab"));
    gtk_box_pack_start(GTK_BOX(tab_actions), tab_add_button, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(tab_actions), tab_close_button, FALSE, FALSE, 0);
    gtk_notebook_set_action_widget(GTK_NOTEBOOK(notebook), tab_actions, GTK_PACK_END);
    gtk_widget_show_all(tab_actions);
    g_signal_connect(tab_add_button, "clicked", G_CALLBACK(on_new_tab), NULL);
    g_signal_connect(tab_close_button, "clicked", G_CALLBACK(on_close_tab), NULL);

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
    g_signal_connect(btn_settings, "clicked", G_CALLBACK(on_settings), NULL);
    g_signal_connect(btn_history, "clicked", G_CALLBACK(on_history), NULL);
    g_signal_connect(btn_bookmark, "clicked", G_CALLBACK(on_bookmark), NULL);
    g_signal_connect(notebook, "switch-page", G_CALLBACK(on_switch_page), NULL);
    g_signal_connect(window, "size-allocate", G_CALLBACK(on_window_size_allocate), NULL);

    apply_ui_language();
    add_tab("https://cn.bing.com");
}

int main(int argc, char* argv[]) {
    g_setenv("WEBKIT_DISABLE_COMPOSITING_MODE", "1", TRUE);
    g_setenv("WEBKIT_DISABLE_DMABUF_RENDERER", "1", TRUE);
    g_setenv("NO_AT_BRIDGE", "1", TRUE);

    gtk_init(&argc, &argv);
    GtkWidget* window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "WebBrowser");
    gtk_window_set_default_size(GTK_WINDOW(window), 1000, 720);
    gtk_widget_set_size_request(window, 320, 240);
    gtk_window_set_resizable(GTK_WINDOW(window), TRUE);
    gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);

    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
    build_ui(window);
    gtk_widget_show_all(window);
    gtk_main();
    return 0;
}
