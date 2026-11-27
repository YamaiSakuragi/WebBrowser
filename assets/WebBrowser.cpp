#include <webkit2/webkit2.h>
#include <gtk/gtk.h>
#include <gdk/gdkkeysyms.h>
#include <cstdio>
#include <cstring>
#include <cctype>
#include <string>
#include <sys/stat.h>
#include <ctime>
#include <vector>
#include <memory>
#include <algorithm>
#include <sstream>
#include <cmath>
#include <cstdlib>
#include <pango/pango.h>

enum IconType {
    ICON_BACK = 0,
    ICON_FORWARD,
    ICON_REFRESH,
    ICON_STAR_OUTLINE,
    ICON_STAR_FILLED,
    ICON_BOOKMARKS,
    ICON_DOWNLOAD,
    ICON_MENU,
    ICON_PLUS,
    ICON_CLOSE,
    ICON_SETTINGS,
    ICON_LIBRARY,
    ICON_HISTORY,
    ICON_COOKIE,
    ICON_GLOBE,
    ICON_APP_LOGO
};

static gboolean on_vector_icon_draw(GtkWidget* widget, cairo_t* cr, gpointer) {
    int icon_val = GPOINTER_TO_INT(g_object_get_data(G_OBJECT(widget), "wb_icon_type"));
    IconType type = static_cast<IconType>(icon_val);
    const int w = gtk_widget_get_allocated_width(widget);
    const int h = gtk_widget_get_allocated_height(widget);
    const double s = std::min(w, h);
    const double cx = w * 0.5;
    const double cy = h * 0.5;

    GtkStyleContext* ctx = gtk_widget_get_style_context(widget);
    GdkRGBA fg;
    gtk_style_context_get_color(ctx, gtk_widget_get_state_flags(widget), &fg);

    cairo_set_line_cap(cr, CAIRO_LINE_CAP_ROUND);
    cairo_set_line_join(cr, CAIRO_LINE_JOIN_ROUND);
    cairo_set_source_rgba(cr, fg.red, fg.green, fg.blue, fg.alpha);

    switch (type) {
        case ICON_BACK: {
            cairo_set_line_width(cr, 1.8);
            double d = s * 0.28;
            cairo_move_to(cr, cx + d, cy);
            cairo_line_to(cr, cx - d, cy);
            cairo_move_to(cr, cx - d * 0.15, cy - d * 0.85);
            cairo_line_to(cr, cx - d, cy);
            cairo_line_to(cr, cx - d * 0.15, cy + d * 0.85);
            cairo_stroke(cr);
            break;
        }
        case ICON_FORWARD: {
            cairo_set_line_width(cr, 1.8);
            double d = s * 0.28;
            cairo_move_to(cr, cx - d, cy);
            cairo_line_to(cr, cx + d, cy);
            cairo_move_to(cr, cx + d * 0.15, cy - d * 0.85);
            cairo_line_to(cr, cx + d, cy);
            cairo_line_to(cr, cx + d * 0.15, cy + d * 0.85);
            cairo_stroke(cr);
            break;
        }
        case ICON_REFRESH: {
            cairo_set_line_width(cr, 1.8);
            double r = s * 0.30;
            cairo_arc(cr, cx, cy, r, -0.35, 1.45 * M_PI);
            cairo_stroke(cr);
            double ax = cx + r * std::cos(-0.35);
            double ay = cy + r * std::sin(-0.35);
            cairo_move_to(cr, ax - r * 0.45, ay - r * 0.15);
            cairo_line_to(cr, ax + r * 0.05, ay + r * 0.1);
            cairo_line_to(cr, ax + r * 0.15, ay - r * 0.45);
            cairo_stroke(cr);
            break;
        }
        case ICON_CLOSE: {
            cairo_set_line_width(cr, 1.5);
            double d = s * 0.24;
            cairo_move_to(cr, cx - d, cy - d);
            cairo_line_to(cr, cx + d, cy + d);
            cairo_move_to(cr, cx + d, cy - d);
            cairo_line_to(cr, cx - d, cy + d);
            cairo_stroke(cr);
            break;
        }
        case ICON_STAR_OUTLINE:
        case ICON_STAR_FILLED: {
            double outer = s * 0.36;
            double inner = s * 0.16;
            for (int i = 0; i < 10; ++i) {
                double ang = -M_PI / 2.0 + i * (M_PI / 5.0);
                double rad = (i % 2 == 0) ? outer : inner;
                double px = cx + rad * std::cos(ang);
                double py = cy + rad * std::sin(ang);
                if (i == 0) cairo_move_to(cr, px, py);
                else cairo_line_to(cr, px, py);
            }
            cairo_close_path(cr);
            if (type == ICON_STAR_FILLED) {
                cairo_set_source_rgba(cr, 0.0, 0.38, 0.87, 1.0);
                cairo_fill_preserve(cr);
            }
            cairo_set_line_width(cr, 1.6);
            cairo_stroke(cr);
            break;
        }
        case ICON_BOOKMARKS: {
            cairo_set_line_width(cr, 1.7);
            double bw = s * 0.24;
            double bh = s * 0.34;
            cairo_move_to(cr, cx - bw, cy - bh);
            cairo_line_to(cr, cx + bw, cy - bh);
            cairo_line_to(cr, cx + bw, cy + bh);
            cairo_line_to(cr, cx, cy + bh * 0.42);
            cairo_line_to(cr, cx - bw, cy + bh);
            cairo_close_path(cr);
            cairo_stroke(cr);
            break;
        }
        case ICON_DOWNLOAD: {
            cairo_set_line_width(cr, 1.8);
            double d = s * 0.26;
            cairo_move_to(cr, cx, cy - d);
            cairo_line_to(cr, cx, cy + d * 0.45);
            cairo_move_to(cr, cx - d * 0.65, cy - d * 0.05);
            cairo_line_to(cr, cx, cy + d * 0.55);
            cairo_line_to(cr, cx + d * 0.65, cy - d * 0.05);
            cairo_move_to(cr, cx - d, cy + d * 0.85);
            cairo_line_to(cr, cx + d, cy + d * 0.85);
            cairo_stroke(cr);
            break;
        }
        case ICON_MENU: {
            cairo_set_line_width(cr, 1.8);
            double d = s * 0.28;
            double dy = s * 0.22;
            cairo_move_to(cr, cx - d, cy - dy); cairo_line_to(cr, cx + d, cy - dy);
            cairo_move_to(cr, cx - d, cy);      cairo_line_to(cr, cx + d, cy);
            cairo_move_to(cr, cx - d, cy + dy); cairo_line_to(cr, cx + d, cy + dy);
            cairo_stroke(cr);
            break;
        }
        case ICON_PLUS: {
            cairo_set_line_width(cr, 1.7);
            double d = s * 0.26;
            cairo_move_to(cr, cx - d, cy); cairo_line_to(cr, cx + d, cy);
            cairo_move_to(cr, cx, cy - d); cairo_line_to(cr, cx, cy + d);
            cairo_stroke(cr);
            break;
        }
        case ICON_SETTINGS: {
            cairo_set_line_width(cr, 1.5);
            double r_out = s * 0.30;
            double r_in = s * 0.14;
            cairo_arc(cr, cx, cy, r_in, 0, 2 * M_PI);
            cairo_stroke(cr);
            cairo_arc(cr, cx, cy, r_out * 0.78, 0, 2 * M_PI);
            cairo_stroke(cr);
            for (int i = 0; i < 6; ++i) {
                double a = i * (M_PI / 3.0);
                cairo_move_to(cr, cx + r_out * 0.72 * std::cos(a), cy + r_out * 0.72 * std::sin(a));
                cairo_line_to(cr, cx + r_out * 1.05 * std::cos(a), cy + r_out * 1.05 * std::sin(a));
            }
            cairo_stroke(cr);
            break;
        }
        case ICON_LIBRARY: {
            cairo_set_line_width(cr, 1.5);
            double d = s * 0.28;
            cairo_rectangle(cr, cx - d, cy - d * 0.85, d * 0.55, d * 1.7);
            cairo_rectangle(cr, cx - d * 0.25, cy - d * 0.85, d * 0.55, d * 1.7);
            cairo_move_to(cr, cx + d * 0.45, cy - d * 0.85);
            cairo_line_to(cr, cx + d * 0.95, cy + d * 0.85);
            cairo_stroke(cr);
            break;
        }
        case ICON_HISTORY: {
            cairo_set_line_width(cr, 1.5);
            double r = s * 0.36;
            cairo_arc(cr, cx, cy, r, 0, 2 * M_PI);
            cairo_stroke(cr);
            cairo_move_to(cr, cx, cy - r * 0.52);
            cairo_line_to(cr, cx, cy);
            cairo_line_to(cr, cx + r * 0.44, cy + r * 0.24);
            cairo_stroke(cr);
            break;
        }
        case ICON_COOKIE: {
            cairo_set_line_width(cr, 1.5);
            double r = s * 0.36;
            cairo_arc(cr, cx, cy, r, 0, 2 * M_PI);
            cairo_stroke(cr);
            double cr_r = std::max(1.1, s * 0.065);
            double pts[4][2] = {
                {-0.38, -0.30},
                { 0.34, -0.26},
                {-0.26,  0.34},
                { 0.32,  0.28}
            };
            for (int i = 0; i < 4; ++i) {
                cairo_arc(cr, cx + r * pts[i][0], cy + r * pts[i][1], cr_r, 0, 2 * M_PI);
                cairo_fill(cr);
            }
            break;
        }
        case ICON_GLOBE: {
            cairo_set_line_width(cr, 1.4);
            double r = s * 0.36;
            cairo_arc(cr, cx, cy, r, 0, 2 * M_PI);
            cairo_stroke(cr);
            cairo_move_to(cr, cx - r, cy);
            cairo_line_to(cr, cx + r, cy);
            cairo_stroke(cr);
            cairo_save(cr);
            cairo_translate(cr, cx, cy);
            cairo_scale(cr, 0.48, 1.0);
            cairo_arc(cr, 0, 0, r, 0, 2 * M_PI);
            cairo_stroke(cr);
            cairo_restore(cr);
            break;
        }
        case ICON_APP_LOGO: {
            double r = s * 0.42;
            cairo_pattern_t* pat = cairo_pattern_create_linear(cx - r, cy - r, cx + r, cy + r);
            cairo_pattern_add_color_stop_rgb(pat, 0.0, 0.0, 0.38, 0.87);
            cairo_pattern_add_color_stop_rgb(pat, 1.0, 0.0, 0.75, 0.95);
            cairo_set_source(cr, pat);
            cairo_arc(cr, cx, cy, r, 0, 2 * M_PI);
            cairo_fill(cr);
            cairo_pattern_destroy(pat);

            cairo_set_source_rgba(cr, 1.0, 1.0, 1.0, 0.92);
            cairo_set_line_width(cr, 2.2);
            cairo_arc(cr, cx, cy, r * 0.62, 0, 2 * M_PI);
            cairo_stroke(cr);
            cairo_move_to(cr, cx - r * 0.62, cy);
            cairo_line_to(cr, cx + r * 0.62, cy);
            cairo_stroke(cr);
            cairo_save(cr);
            cairo_translate(cr, cx, cy);
            cairo_scale(cr, 0.48, 1.0);
            cairo_arc(cr, 0, 0, r * 0.62, 0, 2 * M_PI);
            cairo_stroke(cr);
            cairo_restore(cr);
            break;
        }
    }
    return FALSE;
}

static GtkWidget* create_vector_icon(IconType type, int size_px) {
    GtkWidget* da = gtk_drawing_area_new();
    gtk_widget_set_size_request(da, size_px, size_px);
    g_object_set_data(G_OBJECT(da), "wb_icon_type", GINT_TO_POINTER(static_cast<int>(type)));
    g_signal_connect(da, "draw", G_CALLBACK(on_vector_icon_draw), NULL);
    return da;
}

static void set_vector_icon_type(GtkWidget* da, IconType type) {
    if (!da || !GTK_IS_DRAWING_AREA(da)) return;
    g_object_set_data(G_OBJECT(da), "wb_icon_type", GINT_TO_POINTER(static_cast<int>(type)));
    gtk_widget_queue_draw(da);
}

static gboolean on_favicon_icon_draw(GtkWidget* widget, cairo_t* cr, gpointer) {
    cairo_surface_t* surf = static_cast<cairo_surface_t*>(g_object_get_data(G_OBJECT(widget), "wb_favicon"));
    if (!surf) return FALSE;
    const int sw = cairo_image_surface_get_width(surf);
    const int sh = cairo_image_surface_get_height(surf);
    const int w = gtk_widget_get_allocated_width(widget);
    const int h = gtk_widget_get_allocated_height(widget);
    if (sw > 0 && sh > 0 && w > 0 && h > 0) {
        const double scale = std::min((double)w / (double)sw, (double)h / (double)sh);
        const double ox = (w - sw * scale) * 0.5;
        const double oy = (h - sh * scale) * 0.5;
        cairo_save(cr);
        cairo_translate(cr, ox, oy);
        cairo_scale(cr, scale, scale);
        cairo_set_source_surface(cr, surf, 0, 0);
        cairo_pattern_set_filter(cairo_get_source(cr), CAIRO_FILTER_BILINEAR);
        cairo_paint(cr);
        cairo_restore(cr);
    }
    return FALSE;
}

static GtkWidget* create_favicon_widget(cairo_surface_t* surf, int size_px) {
    GtkWidget* da = gtk_drawing_area_new();
    gtk_widget_set_size_request(da, size_px, size_px);
    gtk_widget_set_halign(da, GTK_ALIGN_CENTER);
    gtk_widget_set_valign(da, GTK_ALIGN_CENTER);
    gtk_widget_set_hexpand(da, FALSE);
    gtk_widget_set_vexpand(da, FALSE);
    cairo_surface_reference(surf);
    g_object_set_data_full(G_OBJECT(da), "wb_favicon", surf, (GDestroyNotify)cairo_surface_destroy);
    g_signal_connect(da, "draw", G_CALLBACK(on_favicon_icon_draw), NULL);
    return da;
}

static void run_selection_toolbar_script(WebKitWebView* view);
void open_preferences_tab(const char* section_name);
void sync_tab_bar();
void update_navigation_state();

static GtkWidget* notebook = nullptr;
static GtkWidget* tab_bar_box = nullptr;
static GtkWidget* url_entry = nullptr;
static GtkWidget* progress_bar = nullptr;
static GtkWidget* status_overlay = nullptr;
static guint status_hide_timer = 0;
static guint resize_timer_id = 0;
static gboolean is_typing = FALSE;

static WebKitWebContext* web_context = nullptr;
static WebKitCookieManager* cookie_manager = nullptr;
static WebKitWebsiteDataManager* data_manager = nullptr;
static WebKitUserContentManager* user_content_manager = nullptr;
static WebKitUserContentFilterStore* content_filter_store = nullptr;

// 独立数据目录
static std::string base_dir = "/data/data/com.termux/files/home/.config/WebBrowser/data";
static std::string history_file = base_dir + "/history/history.txt";
static std::string bookmark_file = base_dir + "/bookmarks/bookmarks.txt";
static std::string downloads_file = base_dir + "/downloads/downloads.txt";
static std::string adblock_file = base_dir + "/adblock/rules.txt";
static std::string app_icon_path = "/data/data/com.termux/files/usr/share/icons/hicolor/64x64/apps/WebBrowser.png";

struct RecordItem {
    std::string time;
    std::string title;
    std::string url;
};
static std::vector<RecordItem> history_list;
static std::vector<RecordItem> bookmark_list;

struct DownloadItem {
    enum class State { InProgress, Finished, Cancelled, Failed };

    int id = 0;
    std::string time;
    std::string filename;
    std::string url;
    std::string destination_path;
    guint64 received_bytes = 0;
    guint64 total_bytes = 0;
    double progress = 0.0;
    gint64 start_time_us = 0;
    gint64 last_speed_time_us = 0;
    guint64 last_speed_bytes = 0;
    double speed_bps = 0.0;
    State state = State::InProgress;
    std::string error_msg;
    bool selected = false;
    WebKitDownload* wk_download = nullptr;

    GtkWidget* row = nullptr;
    GtkWidget* check_btn = nullptr;
    GtkWidget* name_label = nullptr;
    GtkWidget* progress_widget = nullptr;
    GtkWidget* info_label = nullptr;
    GtkWidget* cancel_btn = nullptr;
};

static std::vector<std::shared_ptr<DownloadItem>> download_list;
static int next_download_id = 1;

// 单窗口统一内嵌设置与数据管理子页面（Firefox 风格 about:preferences）
static GtkWidget* preferences_page = nullptr;
static GtkWidget* preferences_sidebar = nullptr;
static GtkWidget* preferences_stack = nullptr;
static std::vector<int> pref_back_stack;
static std::vector<int> pref_forward_stack;
static int pref_current_row = -1;
static bool pref_navigating_history = false;
static GtkWidget* pref_data_toolbar = nullptr;
static bool pref_syncing_ui = false;
static GtkWidget* pref_theme_combo = nullptr;
static GtkWidget* pref_lang_combo = nullptr;
static GtkWidget* pref_zoom_scale = nullptr;
static GtkWidget* pref_gen_action_bar = nullptr;
static GtkWidget* pref_search_combo = nullptr;
static GtkWidget* pref_srch_action_bar = nullptr;
static GtkWidget* pref_dl_entry = nullptr;
static GtkWidget* pref_dl_action_bar = nullptr;
static GtkWidget* pref_cookie_combo = nullptr;
static GtkWidget* pref_media_check = nullptr;
static GtkWidget* pref_tls_check = nullptr;
static GtkWidget* pref_ad_check = nullptr;
static GtkTextBuffer* pref_ad_buf = nullptr;
static GtkWidget* pref_priv_action_bar = nullptr;
static GtkWidget* pref_js_check = nullptr;
static GtkWidget* pref_webgl_check = nullptr;
static GtkWidget* pref_dev_action_bar = nullptr;

static GtkListStore* history_store = nullptr;
static GtkListStore* cookie_store = nullptr;
static GtkListStore* bookmark_store = nullptr;
static GtkWidget* pref_dl_listbox = nullptr;
static GtkWidget* active_download_listbox = nullptr;
static bool active_download_multi_mode = false;



static GtkWidget* dl_popover = nullptr;
static GtkWidget* dl_popover_listbox = nullptr;
static GtkWidget* dl_popover_title = nullptr;
static GtkWidget* dl_popover_manage_btn = nullptr;

static GtkWidget* bm_popover = nullptr;
static GtkWidget* bm_popover_listbox = nullptr;
static GtkWidget* bm_popover_title = nullptr;
static GtkWidget* bm_popover_manage_btn = nullptr;
static GtkWidget* btn_bookmarks_menu = nullptr;

static GtkWidget* record_multi_button = nullptr;
static GtkWidget* record_select_all_button = nullptr;
static GtkWidget* record_invert_button = nullptr;
static GtkWidget* record_delete_button = nullptr;
static GtkWidget* record_copy_button = nullptr;
static GCancellable* cookie_fetch_cancellable = nullptr;
static bool record_multi_mode = false;
static std::vector<GtkTreeViewColumn*> record_selection_columns;

static std::string settings_file = base_dir + "/settings.conf";
static std::string ui_theme = "light"; // "light", "dark"
static std::string ui_language = "zh"; // "zh", "en", "zh-TW"
static std::string search_engine = "google"; // "google", "bing", "baidu"
static std::string cookie_policy = "always"; // "always", "no_third_party", "never"
static bool ignore_tls_errors = true;
static bool adblock_enabled = false;
static std::string adblock_rules_text;
static GtkWidget* adblock_status_lbl = nullptr;
static std::string download_dir = "/data/data/com.termux/files/home/Downloads";
static GtkCssProvider* app_css = nullptr;
static bool app_css_installed = false;
static GtkWidget* main_window = nullptr;
static GtkWidget* btn_back = nullptr;
static GtkWidget* btn_forward = nullptr;
static GtkWidget* btn_refresh = nullptr;
static GtkWidget* icon_refresh = nullptr;
static GtkWidget* btn_bookmark = nullptr;
static GtkWidget* icon_bookmark = nullptr;
static GtkWidget* btn_downloads = nullptr;
static GtkWidget* lbl_downloads_badge = nullptr;
static GtkWidget* btn_menu = nullptr;
static GtkWidget* menu_popover = nullptr;

static GtkWidget* menu_history_btn = nullptr;
static GtkWidget* menu_bookmark_btn = nullptr;
static GtkWidget* menu_downloads_btn = nullptr;
static GtkWidget* menu_cookies_btn = nullptr;
static GtkWidget* menu_settings_btn = nullptr;
static bool javascript_enabled = true;
static bool webgl_enabled = true;
static bool media_requires_gesture = true;
static double page_zoom_percent = 50.0;
static bool zoom_setting_version_seen = false;

struct WebProcessCrashState {
    std::string last_uri;
    gint64 last_time_us = 0;
    int count = 0;
};

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

const char* get_page_type(GtkWidget* page) {
    if (!page || !GTK_IS_WIDGET(page)) return "";
    const char* type = static_cast<const char*>(g_object_get_data(G_OBJECT(page), "wb_page_type"));
    return type ? type : "";
}

GtkWidget* get_webview_from_page(GtkWidget* page) {
    if (!page || !GTK_IS_CONTAINER(page)) return nullptr;
    if (strcmp(get_page_type(page), "web") != 0) return nullptr;
    GList* children = gtk_container_get_children(GTK_CONTAINER(page));
    if (children) {
        GtkWidget* webview = GTK_WIDGET(children->data);
        g_list_free(children);
        return WEBKIT_IS_WEB_VIEW(webview) ? webview : nullptr;
    }
    return nullptr;
}

GtkWidget* get_current_page_widget() {
    if (!notebook || !GTK_IS_NOTEBOOK(notebook)) return nullptr;
    int page_idx = gtk_notebook_get_current_page(GTK_NOTEBOOK(notebook));
    if (page_idx == -1) return nullptr;
    return gtk_notebook_get_nth_page(GTK_NOTEBOOK(notebook), page_idx);
}

GtkWidget* get_current_webview() {
    return get_webview_from_page(get_current_page_widget());
}

std::string now_time_string() {
    time_t now = time(0);
    struct tm* tm_info = localtime(&now);
    char time_buf[64] = "0000-00-00 00:00:00";
    if (tm_info) strftime(time_buf, sizeof(time_buf), "%Y-%m-%d %H:%M:%S", tm_info);
    return time_buf;
}

void ensure_dir_recursive(const std::string& path) {
    if (path.empty()) return;
    std::string current;
    for (size_t i = 0; i < path.size(); ++i) {
        current.push_back(path[i]);
        if (path[i] == '/' || i + 1 == path.size()) {
            if (current.size() > 1) mkdir(current.c_str(), 0777);
        }
    }
}

std::string sanitize_field(const std::string& input) {
    std::string out = input;
    for (char& c : out) {
        if (c == '|' || c == '\n' || c == '\r') c = ' ';
    }
    return out;
}

std::string format_bytes(guint64 bytes) {
    char buf[64];
    if (bytes < 1024ULL) {
        snprintf(buf, sizeof(buf), "%llu B", (unsigned long long)bytes);
    } else if (bytes < 1024ULL * 1024ULL) {
        snprintf(buf, sizeof(buf), "%.1f KB", bytes / 1024.0);
    } else if (bytes < 1024ULL * 1024ULL * 1024ULL) {
        snprintf(buf, sizeof(buf), "%.2f MB", bytes / (1024.0 * 1024.0));
    } else {
        snprintf(buf, sizeof(buf), "%.2f GB", bytes / (1024.0 * 1024.0 * 1024.0));
    }
    return buf;
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

static bool is_webview_visually_finished(WebKitWebView* wv) {
    if (!wv || !WEBKIT_IS_WEB_VIEW(wv)) return true;
    return GPOINTER_TO_INT(g_object_get_data(G_OBJECT(wv), "wb_load_finished_visually")) != 0;
}

static void set_webview_visually_finished(WebKitWebView* wv, bool finished) {
    if (!wv || !WEBKIT_IS_WEB_VIEW(wv)) return;
    g_object_set_data(G_OBJECT(wv), "wb_load_finished_visually", GINT_TO_POINTER(finished ? 1 : 0));
}

static double get_webview_max_progress(WebKitWebView* wv) {
    if (!wv || !WEBKIT_IS_WEB_VIEW(wv)) return 0.0;
    return GPOINTER_TO_INT(g_object_get_data(G_OBJECT(wv), "wb_load_max_progress_milli")) / 1000.0;
}

static void set_webview_max_progress(WebKitWebView* wv, double p) {
    if (!wv || !WEBKIT_IS_WEB_VIEW(wv)) return;
    int milli = (int)(std::max(0.0, std::min(1.0, p)) * 1000.0 + 0.5);
    g_object_set_data(G_OBJECT(wv), "wb_load_max_progress_milli", GINT_TO_POINTER(milli));
}

gboolean hide_progress_if_idle(gpointer) {
    GtkWidget* current = get_current_webview();
    if (progress_bar && GTK_IS_WIDGET(progress_bar) &&
        (!current || !webkit_web_view_is_loading(WEBKIT_WEB_VIEW(current)) ||
         is_webview_visually_finished(WEBKIT_WEB_VIEW(current)))) {
        gtk_widget_hide(progress_bar);
    }
    return G_SOURCE_REMOVE;
}

void on_progress_changed(WebKitWebView* web_view, GParamSpec* pspec, gpointer user_data) {
    if (!web_view || GTK_WIDGET(web_view) != get_current_webview() || !progress_bar) return;
    if (!webkit_web_view_is_loading(web_view) || is_webview_visually_finished(web_view)) {
        gtk_widget_hide(progress_bar);
        return;
    }
    double real_progress = webkit_web_view_get_estimated_load_progress(web_view);
    double prev_max = get_webview_max_progress(web_view);
    double monotonic = std::max(prev_max, real_progress);
    if (monotonic >= 0.999) {
        set_webview_max_progress(web_view, 1.0);
        set_webview_visually_finished(web_view, true);
        gtk_progress_bar_set_fraction(GTK_PROGRESS_BAR(progress_bar), 1.0);
        g_timeout_add(220, hide_progress_if_idle, NULL);
        return;
    }
    if (monotonic > prev_max) {
        set_webview_max_progress(web_view, monotonic);
    }
    gtk_widget_show(progress_bar);
    gtk_progress_bar_set_fraction(GTK_PROGRESS_BAR(progress_bar), monotonic);
    if (status_overlay && !gtk_widget_get_visible(status_overlay))
        update_status_text(tr_full("正在加载...", "正在載入...", "Loading..."), FALSE);
}

void on_is_loading_changed(WebKitWebView* web_view, GParamSpec*, gpointer) {
    if (!web_view || GTK_WIDGET(web_view) != get_current_webview()) return;
    update_navigation_state();
    if (!webkit_web_view_is_loading(web_view)) {
        set_webview_visually_finished(web_view, true);
        if (progress_bar && GTK_IS_WIDGET(progress_bar)) gtk_widget_hide(progress_bar);
        update_status_text(tr_full("完成", "完成", "Done"), TRUE);
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

std::string get_home_url() {
    if (search_engine == "bing") return "https://www.bing.com";
    if (search_engine == "baidu") return "https://www.baidu.com";
    return "https://www.google.com";
}

std::string get_search_url_prefix() {
    if (search_engine == "bing") return "https://www.bing.com/search?q=";
    if (search_engine == "baidu") return "https://www.baidu.com/s?wd=";
    return "https://www.google.com/search?q=";
}

std::string get_search_url(const std::string& query) {
    return get_search_url_prefix() + url_encode(query);
}

std::string trim_whitespace(const std::string& str) {
    size_t start = 0;
    while (start < str.size() && std::isspace(static_cast<unsigned char>(str[start]))) ++start;
    size_t end = str.size();
    while (end > start && std::isspace(static_cast<unsigned char>(str[end - 1]))) --end;
    return str.substr(start, end - start);
}

std::string resolve_input(const char* input) {
    std::string s = trim_whitespace(input ? input : "");
    if (s.empty()) return get_home_url();
    if (s.find(" ") == std::string::npos) {
        if (s.find("://") != std::string::npos) return s;
        if (s.find(".") != std::string::npos) return "https://" + s;
    }
    return get_search_url(s);
}

void read_records(const std::string& file, std::vector<RecordItem>& list) {
    list.clear();
    FILE* f = fopen(file.c_str(), "r");
    if (f) {
        char buf[4096];
        while (fgets(buf, sizeof(buf), f)) {
            std::string line(buf);
            while (!line.empty() && (line.back() == '\n' || line.back() == '\r')) line.pop_back();
            size_t pos1 = line.find('|');
            if (pos1 == std::string::npos) continue;
            size_t pos2 = line.find('|', pos1 + 1);
            if (pos2 != std::string::npos) {
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

void save_downloads() {
    FILE* f = fopen(downloads_file.c_str(), "w");
    if (!f) return;
    for (const auto& item : download_list) {
        int st = static_cast<int>(item->state);
        if (item->state == DownloadItem::State::InProgress) {
            st = static_cast<int>(DownloadItem::State::Cancelled);
        }
        fprintf(f, "%s|%s|%s|%s|%d|%llu|%llu\n",
            sanitize_field(item->time).c_str(),
            sanitize_field(item->filename).c_str(),
            sanitize_field(item->url).c_str(),
            sanitize_field(item->destination_path).c_str(),
            st,
            (unsigned long long)item->received_bytes,
            (unsigned long long)item->total_bytes);
    }
    fclose(f);
}

void load_downloads() {
    download_list.clear();
    FILE* f = fopen(downloads_file.c_str(), "r");
    if (!f) return;
    char buf[4096];
    while (fgets(buf, sizeof(buf), f)) {
        std::string line(buf);
        while (!line.empty() && (line.back() == '\n' || line.back() == '\r')) line.pop_back();
        std::vector<std::string> parts;
        size_t start = 0;
        while (true) {
            size_t pos = line.find('|', start);
            if (pos == std::string::npos) {
                parts.push_back(line.substr(start));
                break;
            }
            parts.push_back(line.substr(start, pos - start));
            start = pos + 1;
        }
        if (parts.size() >= 7) {
            auto item = std::make_shared<DownloadItem>();
            item->id = next_download_id++;
            item->time = parts[0];
            item->filename = parts[1];
            item->url = parts[2];
            item->destination_path = parts[3];
            int st = atoi(parts[4].c_str());
            if (st == static_cast<int>(DownloadItem::State::Finished)) item->state = DownloadItem::State::Finished;
            else if (st == static_cast<int>(DownloadItem::State::Failed)) item->state = DownloadItem::State::Failed;
            else item->state = DownloadItem::State::Cancelled;
            item->received_bytes = strtoull(parts[5].c_str(), nullptr, 10);
            item->total_bytes = strtoull(parts[6].c_str(), nullptr, 10);
            item->progress = (item->total_bytes > 0) ? std::min(1.0, (double)item->received_bytes / (double)item->total_bytes) : (item->state == DownloadItem::State::Finished ? 1.0 : 0.0);
            download_list.push_back(item);
        }
    }
    fclose(f);
}

void refresh_history_ui();

void save_history(const char* url, const char* title) {
    if (!url || strstr(url, "about:blank") || strstr(url, "data:text/html")) return;
    std::string t = (title && strlen(title) > 0) ? title : url;
    t = sanitize_field(t);
    history_list.push_back({now_time_string(), t, url});
    write_records(history_file, history_list);
    refresh_history_ui();
}

void add_tab(const char* url);

gboolean on_decide_policy(WebKitWebView* web_view, WebKitPolicyDecision* decision, WebKitPolicyDecisionType type, gpointer user_data) {
    if (type == WEBKIT_POLICY_DECISION_TYPE_NEW_WINDOW_ACTION) {
        WebKitNavigationPolicyDecision* nav_decision = WEBKIT_NAVIGATION_POLICY_DECISION(decision);
        WebKitNavigationAction* action = webkit_navigation_policy_decision_get_navigation_action(nav_decision);
        WebKitURIRequest* request = webkit_navigation_action_get_request(action);
        const gchar* uri = webkit_uri_request_get_uri(request);

        if (uri && *uri) {
            webkit_policy_decision_ignore(decision);
            add_tab(uri);
            return TRUE;
        }
    } else if (type == WEBKIT_POLICY_DECISION_TYPE_RESPONSE) {
        WebKitResponsePolicyDecision* resp_decision = WEBKIT_RESPONSE_POLICY_DECISION(decision);
        if (!webkit_response_policy_decision_is_mime_type_supported(resp_decision)) {
            webkit_policy_decision_download(decision);
            return TRUE;
        }
    }
    return FALSE;
}

enum RecordColumns { REC_TEXT = 0, REC_URL = 1, REC_TIME = 2, REC_SELECTED = 3, REC_INDEX = 4 };

void apply_privacy_and_security_policies() {
    if (data_manager) {
        webkit_website_data_manager_set_tls_errors_policy(
            data_manager,
            ignore_tls_errors ? WEBKIT_TLS_ERRORS_POLICY_IGNORE : WEBKIT_TLS_ERRORS_POLICY_FAIL
        );
    }
    if (cookie_manager) {
        WebKitCookieAcceptPolicy policy = WEBKIT_COOKIE_POLICY_ACCEPT_ALWAYS;
        if (cookie_policy == "no_third_party") policy = WEBKIT_COOKIE_POLICY_ACCEPT_NO_THIRD_PARTY;
        else if (cookie_policy == "never") policy = WEBKIT_COOKIE_POLICY_ACCEPT_NEVER;
        webkit_cookie_manager_set_accept_policy(cookie_manager, policy);
    }
}

std::string build_adblock_rule_regex(const std::string& raw_rule) {
    std::string r = trim_whitespace(raw_rule);
    if (r.empty() || r[0] == '#' || r[0] == '!') return "";
    if (r.size() >= 2 && r[0] == '|' && r[1] == '|') r = r.substr(2);
    if (!r.empty() && r.back() == '^') r.pop_back();
    if (r.empty()) return "";

    std::string regex;
    for (char c : r) {
        if (c == '*') {
            regex += ".*";
        } else if (c == '.' || c == '+' || c == '?' || c == '(' || c == ')' ||
                   c == '[' || c == ']' || c == '{' || c == '}' || c == '^' ||
                   c == '$' || c == '|' || c == '\\') {
            regex.push_back('\\');
            regex.push_back(c);
        } else {
            regex.push_back(c);
        }
    }
    return ".*" + regex + ".*";
}

std::string json_escape_str(const std::string& s) {
    std::string out;
    for (char c : s) {
        if (c == '\\') out += "\\\\";
        else if (c == '"') out += "\\\"";
        else if (c == '\n' || c == '\r') continue;
        else out.push_back(c);
    }
    return out;
}

void load_adblock_rules() {
    adblock_rules_text.clear();
    FILE* f = fopen(adblock_file.c_str(), "r");
    if (!f) return;
    char buf[4096];
    while (fgets(buf, sizeof(buf), f)) {
        adblock_rules_text += buf;
    }
    fclose(f);
}

void save_adblock_rules(const std::string& text) {
    adblock_rules_text = text;
    FILE* f = fopen(adblock_file.c_str(), "w");
    if (f) {
        fwrite(text.data(), 1, text.size(), f);
        fclose(f);
    }
}

void on_adblock_filter_saved(GObject* source, GAsyncResult* res, gpointer) {
    GError* err = nullptr;
    WebKitUserContentFilter* filter = webkit_user_content_filter_store_save_finish(
        WEBKIT_USER_CONTENT_FILTER_STORE(source), res, &err);
    if (filter) {
        if (user_content_manager) {
            webkit_user_content_manager_remove_all_filters(user_content_manager);
            webkit_user_content_manager_add_filter(user_content_manager, filter);
        }
        webkit_user_content_filter_unref(filter);
    }
    if (err) {
        g_error_free(err);
    }
}

void apply_adblock_rules() {
    if (!user_content_manager) return;
    if (!adblock_enabled) {
        webkit_user_content_manager_remove_all_filters(user_content_manager);
        if (adblock_status_lbl && GTK_IS_LABEL(adblock_status_lbl)) {
            gtk_label_set_text(GTK_LABEL(adblock_status_lbl),
                tr_full("广告拦截已关闭", "廣告攔截已關閉", "Ad blocker is disabled"));
        }
        return;
    }

    std::vector<std::string> regexes;
    std::istringstream iss(adblock_rules_text);
    std::string line;
    while (std::getline(iss, line)) {
        std::string rx = build_adblock_rule_regex(line);
        if (!rx.empty()) regexes.push_back(rx);
    }

    if (regexes.empty()) {
        webkit_user_content_manager_remove_all_filters(user_content_manager);
        if (adblock_status_lbl && GTK_IS_LABEL(adblock_status_lbl)) {
            gtk_label_set_text(GTK_LABEL(adblock_status_lbl),
                tr_full("当前无规则（请在上方输入域名或关键词）", "目前無規則（請在上方輸入網域或關鍵字）", "No rules yet (add domains or keywords above)"));
        }
        return;
    }

    std::string json = "[";
    for (size_t i = 0; i < regexes.size(); ++i) {
        if (i > 0) json += ",";
        json += "{\"trigger\":{\"url-filter\":\"" + json_escape_str(regexes[i]) + "\"},\"action\":{\"type\":\"block\"}}";
    }
    json += "]";

    if (content_filter_store) {
        GBytes* bytes = g_bytes_new(json.data(), json.size());
        webkit_user_content_filter_store_save(content_filter_store, "user_adblock", bytes, NULL, on_adblock_filter_saved, NULL);
        g_bytes_unref(bytes);
    }
    if (adblock_status_lbl && GTK_IS_LABEL(adblock_status_lbl)) {
        std::string msg = tr_full("已启用 ", "已啟用 ", "Active: ") + std::to_string(regexes.size()) + tr_full(" 条拦截规则", " 條攔截規則", " rules");
        gtk_label_set_text(GTK_LABEL(adblock_status_lbl), msg.c_str());
    }
}

void apply_app_theme() {
    if (!app_css) app_css = gtk_css_provider_new();
    const char* css_light =
        "window, popover { background-color: #f9f9fb; color: #15141a; }"
        "button { background-image: none; background-color: #f0f0f4; color: #15141a; border: 1px solid #cfcfd8; border-radius: 6px; padding: 5px 10px; }"
        "button:hover { background-color: #e0e0e6; }"
        "button:active { background-color: #d0d0d8; }"
        "button:focus { outline-style: none; }"
        "button.flat { background-color: transparent; border-color: transparent; }"
        "button.flat:hover { background-color: #e0e0e6; }"
        ".tab-strip { background-color: #f0f0f4; padding: 5px 8px; border-bottom: 1px solid #d7d7de; }"
        ".tab-pill { background-color: transparent; border-radius: 7px; padding: 2px 4px; }"
        ".tab-pill:hover { background-color: #e3e3ea; }"
        ".tab-pill-active { background-color: #ffffff; border: 1px solid #d2d2db; border-radius: 7px; padding: 2px 4px; box-shadow: 0 1px 3px rgba(0,0,0,0.08); }"
        ".nav-toolbar { background-color: #ffffff; border-bottom: 1px solid #dcdce4; padding: 6px 8px; }"
        ".omnibox { background-color: #f0f0f4; color: #15141a; border: 1px solid transparent; border-radius: 6px; padding: 5px 10px; }"
        ".omnibox:focus { background-color: #ffffff; border-color: #0060df; }"
        ".menu-item-btn { border: none; border-radius: 5px; padding: 7px 12px; background-color: transparent; }"
        ".menu-item-btn:hover { background-color: #e7e7ee; }"
        ".fx-sidebar { background-color: #f9f9fb; border-right: 1px solid #e0e0e6; padding: 12px 8px; }"
        ".fx-sidebar row { border-radius: 6px; padding: 8px 12px; margin: 2px 0; background-color: transparent; }"
        ".fx-sidebar row:selected { background-color: #e0ecff; color: #0060df; font-weight: bold; }"
        ".fx-page-title { font-size: 20px; font-weight: bold; }"
        ".fx-section-title { font-size: 14px; font-weight: bold; margin-bottom: 4px; }"
        ".fx-card { background-color: #ffffff; border: 1px solid #e0e0e6; border-radius: 8px; padding: 16px; }"
        "entry, textview, text, treeview, list { background-color: #ffffff; color: #15141a; }"
        "checkbutton, radiobutton, label { color: #15141a; }"
        "frame { border-color: #dcdce4; }"
        "scrolledwindow { background-color: #f9f9fb; }"
        "scale trough { background-color: #d7d7de; }";
    const char* css_dark =
        "window, popover { background-color: #1c1b22; color: #fbfbfe; }"
        "button { background-image: none; background-color: #2b2a33; color: #fbfbfe; border: 1px solid #42414d; border-radius: 6px; padding: 5px 10px; }"
        "button:hover { background-color: #3a3944; }"
        "button:active { background-color: #4a4957; }"
        "button:focus { outline-style: none; }"
        "button.flat { background-color: transparent; border-color: transparent; }"
        "button.flat:hover { background-color: #383742; }"
        ".tab-strip { background-color: #1c1b22; padding: 5px 8px; border-bottom: 1px solid #2b2a33; }"
        ".tab-pill { background-color: transparent; border-radius: 7px; padding: 2px 4px; }"
        ".tab-pill:hover { background-color: #2c2b35; }"
        ".tab-pill-active { background-color: #42414d; border: 1px solid #525160; border-radius: 7px; padding: 2px 4px; }"
        ".nav-toolbar { background-color: #2b2a33; border-bottom: 1px solid #1c1b22; padding: 6px 8px; }"
        ".omnibox { background-color: #1c1b22; color: #fbfbfe; border: 1px solid transparent; border-radius: 6px; padding: 5px 10px; }"
        ".omnibox:focus { background-color: #23222b; border-color: #00ddff; }"
        ".menu-item-btn { border: none; border-radius: 5px; padding: 7px 12px; background-color: transparent; }"
        ".menu-item-btn:hover { background-color: #3a3944; }"
        ".fx-sidebar { background-color: #1c1b22; border-right: 1px solid #32313c; padding: 12px 8px; }"
        ".fx-sidebar row { border-radius: 6px; padding: 8px 12px; margin: 2px 0; background-color: transparent; }"
        ".fx-sidebar row:selected { background-color: #283a4e; color: #75d8ff; font-weight: bold; }"
        ".fx-page-title { font-size: 20px; font-weight: bold; }"
        ".fx-section-title { font-size: 14px; font-weight: bold; margin-bottom: 4px; }"
        ".fx-card { background-color: #23222b; border: 1px solid #383745; border-radius: 8px; padding: 16px; }"
        "entry, textview, text, treeview, list { background-color: #23222b; color: #fbfbfe; }"
        "checkbutton, radiobutton, label { color: #fbfbfe; }"
        "frame { border-color: #383745; }"
        "scrolledwindow { background-color: #1c1b22; }"
        "scale trough { background-color: #3a3944; }";
    gtk_css_provider_load_from_data(app_css, ui_theme == "dark" ? css_dark : css_light, -1, NULL);
    GdkScreen* screen = gdk_screen_get_default();
    if (screen && !app_css_installed) {
        gtk_style_context_add_provider_for_screen(screen, GTK_STYLE_PROVIDER(app_css), GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
        app_css_installed = true;
    }
    // 采用标准 GTK 深浅色偏好信号：支持 prefers-color-scheme 的网页会自然适配，不支持的保持原样，不强行覆盖网页样式
    GtkSettings* settings = gtk_settings_get_default();
    if (settings) g_object_set(settings, "gtk-application-prefer-dark-theme", ui_theme == "dark", NULL);
    if (main_window) gtk_widget_queue_draw(main_window);
}

void update_download_button_badge() {
    if (!lbl_downloads_badge) return;
    int active_count = 0;
    for (const auto& item : download_list) {
        if (item->state == DownloadItem::State::InProgress) active_count++;
    }
    if (active_count > 0) {
        gtk_label_set_text(GTK_LABEL(lbl_downloads_badge), std::to_string(active_count).c_str());
        gtk_widget_show(lbl_downloads_badge);
    } else {
        gtk_widget_hide(lbl_downloads_badge);
    }
}

void update_navigation_state() {
    GtkWidget* cur_page = get_current_page_widget();
    const char* ptype = get_page_type(cur_page);

    if (strcmp(ptype, "preferences") == 0) {
        if (btn_back) gtk_widget_set_sensitive(btn_back, !pref_back_stack.empty());
        if (btn_forward) gtk_widget_set_sensitive(btn_forward, !pref_forward_stack.empty());
        const bool is_data_subpage = (pref_current_row >= 5 && pref_current_row <= 8);
        if (btn_refresh) {
            if (is_data_subpage) {
                gtk_widget_show(btn_refresh);
                gtk_widget_set_sensitive(btn_refresh, TRUE);
                set_vector_icon_type(icon_refresh, ICON_REFRESH);
                gtk_widget_set_tooltip_text(btn_refresh, tr_full("刷新当前列表", "重新整理目前列表", "Refresh list"));
            } else {
                gtk_widget_hide(btn_refresh);
            }
        }
        if (btn_bookmark) gtk_widget_hide(btn_bookmark);
        return;
    }

    GtkWidget* wv = get_webview_from_page(cur_page);
    if (!wv) return;

    WebKitWebView* webview = WEBKIT_WEB_VIEW(wv);
    if (btn_back) gtk_widget_set_sensitive(btn_back, webkit_web_view_can_go_back(webview));
    if (btn_forward) gtk_widget_set_sensitive(btn_forward, webkit_web_view_can_go_forward(webview));
    if (btn_refresh) {
        gtk_widget_show(btn_refresh);
        gtk_widget_set_sensitive(btn_refresh, TRUE);
        set_vector_icon_type(icon_refresh, ICON_REFRESH);
        gtk_widget_set_tooltip_text(btn_refresh, tr_full("刷新", "重新整理", "Reload"));
    }
    if (btn_bookmark) {
        gtk_widget_show(btn_bookmark);
        gtk_widget_set_sensitive(btn_bookmark, TRUE);
        const gchar* uri = webkit_web_view_get_uri(webview);
        bool bookmarked = false;
        if (uri && *uri) {
            for (const auto& item : bookmark_list) {
                if (item.url == uri) { bookmarked = true; break; }
            }
        }
        set_vector_icon_type(icon_bookmark, bookmarked ? ICON_STAR_FILLED : ICON_STAR_OUTLINE);
        gtk_widget_set_tooltip_text(btn_bookmark, bookmarked
            ? tr_full("已收藏此页面", "已收藏此頁面", "Bookmarked")
            : tr_full("收藏当前页面", "收藏目前頁面", "Bookmark this page"));
    }
}

void apply_ui_language() {
    if (btn_back) gtk_widget_set_tooltip_text(btn_back, tr_full("后退", "後退", "Back"));
    if (btn_forward) gtk_widget_set_tooltip_text(btn_forward, tr_full("前进", "前進", "Forward"));
    if (btn_bookmarks_menu) gtk_widget_set_tooltip_text(btn_bookmarks_menu, tr_full("书签", "書籤", "Bookmarks"));
    if (btn_downloads) gtk_widget_set_tooltip_text(btn_downloads, tr_full("下载", "下載", "Downloads"));
    if (btn_menu) gtk_widget_set_tooltip_text(btn_menu, tr_full("打开应用菜单", "開啟應用選單", "Open application menu"));
    if (url_entry) gtk_entry_set_placeholder_text(GTK_ENTRY(url_entry), tr_full("搜索或输入网址", "搜尋或輸入網址", "Search or enter address"));
    if (bm_popover_title) gtk_label_set_text(GTK_LABEL(bm_popover_title), tr_full("书签", "書籤", "Bookmarks"));
    if (bm_popover_manage_btn) gtk_button_set_label(GTK_BUTTON(bm_popover_manage_btn), tr_full("打开书签管理", "開啟書籤管理", "Open Bookmarks Manager"));
    if (dl_popover_title) gtk_label_set_text(GTK_LABEL(dl_popover_title), tr_full("最近下载", "最近下載", "Recent downloads"));
    if (dl_popover_manage_btn) gtk_button_set_label(GTK_BUTTON(dl_popover_manage_btn), tr_full("打开下载管理", "開啟下載管理", "Open Downloads Manager"));
    if (menu_history_btn) gtk_button_set_label(GTK_BUTTON(menu_history_btn), tr_full("历史记录", "歷史記錄", "History"));
    if (menu_bookmark_btn) gtk_button_set_label(GTK_BUTTON(menu_bookmark_btn), tr_full("书签管理", "書籤管理", "Bookmarks"));
    if (menu_downloads_btn) gtk_button_set_label(GTK_BUTTON(menu_downloads_btn), tr_full("下载管理", "下載管理", "Downloads"));
    if (menu_cookies_btn) gtk_button_set_label(GTK_BUTTON(menu_cookies_btn), "Cookies");
    if (menu_settings_btn) gtk_button_set_label(GTK_BUTTON(menu_settings_btn), tr_full("设置", "設定", "Settings"));

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
    update_navigation_state();
    sync_tab_bar();
}

void load_settings() {
    const char* xdg_dl = g_get_user_special_dir(G_USER_DIRECTORY_DOWNLOAD);
    if (xdg_dl && *xdg_dl) download_dir = xdg_dl;

    FILE* f = fopen(settings_file.c_str(), "r");
    if (!f) return;
    char buf[1024];
    while (fgets(buf, sizeof(buf), f)) {
        std::string line(buf);
        while (!line.empty() && (line.back() == '\n' || line.back() == '\r')) line.pop_back();
        size_t eq = line.find('=');
        if (eq == std::string::npos) continue;
        std::string key = trim_whitespace(line.substr(0, eq));
        std::string value = trim_whitespace(line.substr(eq + 1));
        if (key == "theme" && (value == "light" || value == "dark")) ui_theme = value;
        else if (key == "language" && (value == "zh" || value == "en" || value == "zh-TW")) ui_language = value;
        else if (key == "search_engine" && (value == "google" || value == "bing" || value == "baidu")) search_engine = value;
        else if (key == "cookie_policy" && (value == "always" || value == "no_third_party" || value == "never")) cookie_policy = value;
        else if (key == "ignore_tls_errors") ignore_tls_errors = (value == "1");
        else if (key == "adblock_enabled") adblock_enabled = (value == "1");
        else if (key == "download_dir" && !value.empty()) download_dir = value;
        else if (key == "javascript_enabled") javascript_enabled = (value == "1");
        else if (key == "webgl_enabled") webgl_enabled = (value == "1");
        else if (key == "media_requires_gesture") media_requires_gesture = (value == "1");
        else if (key == "page_zoom_percent") page_zoom_percent = std::max(0.0, std::min(100.0, atof(value.c_str())));
        else if (key == "zoom_setting_version") zoom_setting_version_seen = (value == "4");
    }
    fclose(f);
    if (!zoom_setting_version_seen) page_zoom_percent = 50.0;
}

void save_settings() {
    FILE* f = fopen(settings_file.c_str(), "w");
    if (!f) return;
    fprintf(f, "theme=%s\nlanguage=%s\nsearch_engine=%s\ncookie_policy=%s\nignore_tls_errors=%d\nadblock_enabled=%d\ndownload_dir=%s\njavascript_enabled=%d\nwebgl_enabled=%d\nmedia_requires_gesture=%d\npage_zoom_percent=%.0f\nzoom_setting_version=4\n",
        ui_theme.c_str(), ui_language.c_str(), search_engine.c_str(), cookie_policy.c_str(),
        ignore_tls_errors ? 1 : 0, adblock_enabled ? 1 : 0, download_dir.c_str(),
        javascript_enabled ? 1 : 0, webgl_enabled ? 1 : 0, media_requires_gesture ? 1 : 0, page_zoom_percent);
    fclose(f);
}

void refresh_download_row_ui(const std::shared_ptr<DownloadItem>& item) {
    if (!item || !active_download_listbox) return;
    if (item->name_label && GTK_IS_LABEL(item->name_label)) {
        gtk_label_set_text(GTK_LABEL(item->name_label), item->filename.c_str());
    }
    if (item->progress_widget && GTK_IS_PROGRESS_BAR(item->progress_widget)) {
        if (item->state == DownloadItem::State::InProgress) {
            gtk_widget_show(item->progress_widget);
            if (item->total_bytes > 0) {
                gtk_progress_bar_set_fraction(GTK_PROGRESS_BAR(item->progress_widget), std::max(0.0, std::min(1.0, item->progress)));
            } else {
                gtk_progress_bar_pulse(GTK_PROGRESS_BAR(item->progress_widget));
            }
        } else {
            gtk_widget_hide(item->progress_widget);
        }
    }
    if (item->info_label && GTK_IS_LABEL(item->info_label)) {
        std::string info;
        if (item->state == DownloadItem::State::InProgress) {
            std::string recv_str = format_bytes(item->received_bytes);
            std::string spd_str = format_bytes((guint64)std::max(0.0, item->speed_bps)) + "/s";
            if (item->total_bytes > 0) {
                int pct = (int)std::round(item->progress * 100.0);
                info = recv_str + " / " + format_bytes(item->total_bytes) + "  ·  " + spd_str + "  ·  " + std::to_string(pct) + "%";
            } else {
                info = recv_str + "  ·  " + spd_str;
            }
        } else if (item->state == DownloadItem::State::Finished) {
            guint64 final_size = item->total_bytes > 0 ? item->total_bytes : item->received_bytes;
            info = format_bytes(final_size) + "  ·  " + tr_full("已完成", "已完成", "Completed") + "  ·  " + item->destination_path;
        } else if (item->state == DownloadItem::State::Cancelled) {
            info = tr_full("已取消", "已取消", "Cancelled") + std::string("  ·  ") + item->time;
        } else {
            info = tr_full("下载失败", "下載失敗", "Failed") + (!item->error_msg.empty() ? ": " + item->error_msg : "");
        }
        gtk_label_set_text(GTK_LABEL(item->info_label), info.c_str());
    }
    if (item->cancel_btn && GTK_IS_WIDGET(item->cancel_btn)) {
        gtk_widget_set_visible(item->cancel_btn, item->state == DownloadItem::State::InProgress);
    }
    if (item->check_btn && GTK_IS_TOGGLE_BUTTON(item->check_btn)) {
        const bool can_select = (item->state != DownloadItem::State::InProgress);
        if (!can_select) item->selected = false;
        gtk_widget_set_visible(item->check_btn, active_download_multi_mode);
        gtk_widget_set_sensitive(item->check_btn, can_select);
        gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(item->check_btn), can_select && item->selected);
    }
}

void clear_download_row_widgets() {
    for (const auto& item : download_list) {
        item->row = nullptr;
        item->check_btn = nullptr;
        item->name_label = nullptr;
        item->progress_widget = nullptr;
        item->info_label = nullptr;
        item->cancel_btn = nullptr;
    }
}

void rebuild_download_list_ui(GtkWidget* target_listbox, bool multi_mode) {
    if (!target_listbox || !GTK_IS_LIST_BOX(target_listbox)) return;
    clear_download_row_widgets();
    active_download_listbox = target_listbox;
    active_download_multi_mode = multi_mode;

    GList* children = gtk_container_get_children(GTK_CONTAINER(target_listbox));
    for (GList* l = children; l != nullptr; l = l->next) {
        gtk_widget_destroy(GTK_WIDGET(l->data));
    }
    g_list_free(children);

    if (download_list.empty()) {
        GtkWidget* empty_row = gtk_list_box_row_new();
        gtk_list_box_row_set_activatable(GTK_LIST_BOX_ROW(empty_row), FALSE);
        gtk_list_box_row_set_selectable(GTK_LIST_BOX_ROW(empty_row), FALSE);
        GtkWidget* lbl = gtk_label_new(tr_full("暂无下载记录", "暫無下載記錄", "No downloads yet"));
        gtk_container_set_border_width(GTK_CONTAINER(lbl), 18);
        gtk_container_add(GTK_CONTAINER(empty_row), lbl);
        gtk_widget_show_all(empty_row);
        gtk_list_box_insert(GTK_LIST_BOX(target_listbox), empty_row, -1);
        return;
    }

    for (auto it = download_list.rbegin(); it != download_list.rend(); ++it) {
        auto item = *it;
        item->row = gtk_list_box_row_new();
        gtk_list_box_row_set_activatable(GTK_LIST_BOX_ROW(item->row), FALSE);
        gtk_list_box_row_set_selectable(GTK_LIST_BOX_ROW(item->row), FALSE);

        GtkWidget* hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 8);
        gtk_container_set_border_width(GTK_CONTAINER(hbox), 8);

        item->check_btn = gtk_check_button_new();
        gtk_widget_set_valign(item->check_btn, GTK_ALIGN_CENTER);
        auto* check_holder = new std::weak_ptr<DownloadItem>(item);
        g_signal_connect_data(item->check_btn, "toggled", G_CALLBACK(+[](GtkToggleButton* btn, gpointer data) {
            auto* wp = static_cast<std::weak_ptr<DownloadItem>*>(data);
            if (auto sp = wp->lock()) {
                if (sp->state == DownloadItem::State::InProgress) {
                    sp->selected = false;
                    if (gtk_toggle_button_get_active(btn)) gtk_toggle_button_set_active(btn, FALSE);
                } else {
                    sp->selected = gtk_toggle_button_get_active(btn);
                }
            }
        }), check_holder, +[](gpointer data, GClosure*) {
            delete static_cast<std::weak_ptr<DownloadItem>*>(data);
        }, (GConnectFlags)0);
        gtk_box_pack_start(GTK_BOX(hbox), item->check_btn, FALSE, FALSE, 0);

        GtkWidget* mid_vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 4);
        gtk_widget_set_hexpand(mid_vbox, TRUE);

        item->name_label = gtk_label_new(item->filename.c_str());
        gtk_label_set_xalign(GTK_LABEL(item->name_label), 0.0f);
        gtk_label_set_ellipsize(GTK_LABEL(item->name_label), PANGO_ELLIPSIZE_MIDDLE);
        gtk_label_set_width_chars(GTK_LABEL(item->name_label), 14);
        gtk_label_set_max_width_chars(GTK_LABEL(item->name_label), 28);
        gtk_widget_set_hexpand(item->name_label, TRUE);
        gtk_box_pack_start(GTK_BOX(mid_vbox), item->name_label, FALSE, FALSE, 0);

        item->progress_widget = gtk_progress_bar_new();
        gtk_widget_set_hexpand(item->progress_widget, TRUE);
        gtk_box_pack_start(GTK_BOX(mid_vbox), item->progress_widget, FALSE, FALSE, 0);

        item->info_label = gtk_label_new("");
        gtk_label_set_xalign(GTK_LABEL(item->info_label), 0.0f);
        gtk_label_set_ellipsize(GTK_LABEL(item->info_label), PANGO_ELLIPSIZE_END);
        gtk_label_set_width_chars(GTK_LABEL(item->info_label), 14);
        gtk_label_set_max_width_chars(GTK_LABEL(item->info_label), 32);
        gtk_widget_set_hexpand(item->info_label, TRUE);
        gtk_box_pack_start(GTK_BOX(mid_vbox), item->info_label, FALSE, FALSE, 0);

        gtk_box_pack_start(GTK_BOX(hbox), mid_vbox, TRUE, TRUE, 0);

        item->cancel_btn = gtk_button_new();
        gtk_container_add(GTK_CONTAINER(item->cancel_btn), create_vector_icon(ICON_CLOSE, 14));
        gtk_widget_set_valign(item->cancel_btn, GTK_ALIGN_CENTER);
        gtk_widget_set_tooltip_text(item->cancel_btn, tr_full("终止下载", "終止下載", "Cancel download"));
        auto* cancel_holder = new std::weak_ptr<DownloadItem>(item);
        g_signal_connect_data(item->cancel_btn, "clicked", G_CALLBACK(+[](GtkButton*, gpointer data) {
            auto* wp = static_cast<std::weak_ptr<DownloadItem>*>(data);
            if (auto sp = wp->lock()) {
                if (sp->state == DownloadItem::State::InProgress && sp->wk_download) {
                    webkit_download_cancel(sp->wk_download);
                }
            }
        }), cancel_holder, +[](gpointer data, GClosure*) {
            delete static_cast<std::weak_ptr<DownloadItem>*>(data);
        }, (GConnectFlags)0);
        gtk_box_pack_end(GTK_BOX(hbox), item->cancel_btn, FALSE, FALSE, 0);

        gtk_container_add(GTK_CONTAINER(item->row), hbox);
        gtk_widget_show_all(item->row);
        refresh_download_row_ui(item);
        gtk_list_box_insert(GTK_LIST_BOX(target_listbox), item->row, -1);
    }
}

static std::string extract_filename_from_url(const std::string& url) {
    if (url.empty()) return "";
    std::string clean = url;
    size_t q = clean.find_first_of("?#");
    if (q != std::string::npos) clean = clean.substr(0, q);
    size_t slash = clean.rfind('/');
    if (slash != std::string::npos && slash + 1 < clean.size()) {
        return clean.substr(slash + 1);
    }
    return "";
}

static std::string make_unique_download_path(const std::string& dir, const std::string& raw_filename) {
    std::string clean_name = raw_filename.empty() ? "download" : raw_filename;
    for (char& c : clean_name) {
        if (c == '/' || c == '\\' || c == '\0') c = '_';
    }
    ensure_dir_recursive(dir);
    std::string base_path = dir;
    if (!base_path.empty() && base_path.back() != '/') base_path.push_back('/');

    std::string full = base_path + clean_name;
    struct stat st;
    if (stat(full.c_str(), &st) != 0) return full;

    size_t dot = clean_name.rfind('.');
    std::string stem = (dot != std::string::npos && dot > 0) ? clean_name.substr(0, dot) : clean_name;
    std::string ext = (dot != std::string::npos && dot > 0) ? clean_name.substr(dot) : "";
    for (int i = 1; i < 10000; ++i) {
        std::string candidate = base_path + stem + " (" + std::to_string(i) + ")" + ext;
        if (stat(candidate.c_str(), &st) != 0) return candidate;
    }
    return full;
}

void on_download_started(WebKitWebContext*, WebKitDownload* download, gpointer) {
    if (!download) return;
    auto item = std::make_shared<DownloadItem>();
    item->id = next_download_id++;
    item->time = now_time_string();
    item->start_time_us = g_get_monotonic_time();
    item->last_speed_time_us = item->start_time_us;
    item->state = DownloadItem::State::InProgress;
    item->wk_download = WEBKIT_DOWNLOAD(g_object_ref(download));

    WebKitURIRequest* req = webkit_download_get_request(download);
    const gchar* uri = req ? webkit_uri_request_get_uri(req) : nullptr;
    item->url = uri ? uri : "";
    item->filename = tr_full("正在获取文件名...", "正在取得檔案名稱...", "Fetching filename...");
    download_list.push_back(item);
    update_download_button_badge();

    if (active_download_listbox) {
        rebuild_download_list_ui(active_download_listbox, active_download_multi_mode);
    }

    auto* decide_holder = new std::shared_ptr<DownloadItem>(item);
    g_signal_connect_data(download, "decide-destination", G_CALLBACK(+[](WebKitDownload* dl, const gchar* suggested_filename, gpointer data) -> gboolean {
        auto* sp = static_cast<std::shared_ptr<DownloadItem>*>(data);
        auto item = *sp;
        std::string fname = (suggested_filename && *suggested_filename) ? suggested_filename : extract_filename_from_url(item->url);
        if (fname.empty()) fname = "download";
        std::string dest_path = make_unique_download_path(download_dir, fname);
        size_t slash = dest_path.rfind('/');
        item->destination_path = dest_path;
        item->filename = (slash != std::string::npos) ? dest_path.substr(slash + 1) : dest_path;
        gchar* file_uri = g_filename_to_uri(dest_path.c_str(), nullptr, nullptr);
        if (file_uri) {
            webkit_download_set_destination(dl, file_uri);
            g_free(file_uri);
        }
        update_status_text((std::string(tr_full("开始下载: ", "開始下載: ", "Downloading: ")) + item->filename).c_str(), TRUE);
        refresh_download_row_ui(item);
        return TRUE;
    }), decide_holder, +[](gpointer data, GClosure*) {
        delete static_cast<std::shared_ptr<DownloadItem>*>(data);
    }, (GConnectFlags)0);

    auto* recv_holder = new std::shared_ptr<DownloadItem>(item);
    g_signal_connect_data(download, "received-data", G_CALLBACK(+[](WebKitDownload* dl, guint64, gpointer data) {
        auto* sp = static_cast<std::shared_ptr<DownloadItem>*>(data);
        auto item = *sp;
        item->received_bytes = webkit_download_get_received_data_length(dl);
        item->progress = webkit_download_get_estimated_progress(dl);
        WebKitURIResponse* resp = webkit_download_get_response(dl);
        if (resp) {
            guint64 cl = webkit_uri_response_get_content_length(resp);
            if (cl > 0) item->total_bytes = cl;
        }
        gint64 now_us = g_get_monotonic_time();
        gint64 dt = now_us - item->last_speed_time_us;
        if (dt >= 400000) {
            guint64 dbytes = (item->received_bytes >= item->last_speed_bytes) ? (item->received_bytes - item->last_speed_bytes) : 0;
            item->speed_bps = (double)dbytes / ((double)dt / (double)G_USEC_PER_SEC);
            item->last_speed_time_us = now_us;
            item->last_speed_bytes = item->received_bytes;
        }
        refresh_download_row_ui(item);
    }), recv_holder, +[](gpointer data, GClosure*) {
        delete static_cast<std::shared_ptr<DownloadItem>*>(data);
    }, (GConnectFlags)0);

    auto* fin_holder = new std::shared_ptr<DownloadItem>(item);
    g_signal_connect_data(download, "finished", G_CALLBACK(+[](WebKitDownload* dl, gpointer data) {
        auto* sp = static_cast<std::shared_ptr<DownloadItem>*>(data);
        auto item = *sp;
        if (item->state == DownloadItem::State::InProgress) {
            item->state = DownloadItem::State::Finished;
            item->received_bytes = webkit_download_get_received_data_length(dl);
            if (item->total_bytes == 0) item->total_bytes = item->received_bytes;
            item->progress = 1.0;
            update_status_text((std::string(tr_full("下载完成: ", "下載完成: ", "Download finished: ")) + item->filename).c_str(), TRUE);
        }
        if (item->wk_download) {
            g_object_unref(item->wk_download);
            item->wk_download = nullptr;
        }
        save_downloads();
        update_download_button_badge();
        refresh_download_row_ui(item);
    }), fin_holder, +[](gpointer data, GClosure*) {
        delete static_cast<std::shared_ptr<DownloadItem>*>(data);
    }, (GConnectFlags)0);

    auto* fail_holder = new std::shared_ptr<DownloadItem>(item);
    g_signal_connect_data(download, "failed", G_CALLBACK(+[](WebKitDownload* dl, GError* error, gpointer data) {
        auto* sp = static_cast<std::shared_ptr<DownloadItem>*>(data);
        auto item = *sp;
        if (error && g_error_matches(error, WEBKIT_DOWNLOAD_ERROR, WEBKIT_DOWNLOAD_ERROR_CANCELLED_BY_USER)) {
            item->state = DownloadItem::State::Cancelled;
            update_status_text((std::string(tr_full("已取消下载: ", "已取消下載: ", "Download cancelled: ")) + item->filename).c_str(), TRUE);
        } else {
            item->state = DownloadItem::State::Failed;
            item->error_msg = (error && error->message) ? error->message : "Unknown error";
            update_status_text((std::string(tr_full("下载失败: ", "下載失敗: ", "Download failed: ")) + item->filename).c_str(), TRUE);
        }
        if (item->wk_download) {
            g_object_unref(item->wk_download);
            item->wk_download = nullptr;
        }
        save_downloads();
        update_download_button_badge();
        refresh_download_row_ui(item);
    }), fail_holder, +[](gpointer data, GClosure*) {
        delete static_cast<std::shared_ptr<DownloadItem>*>(data);
    }, (GConnectFlags)0);
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

void rebuild_bookmark_popover_ui() {
    if (!bm_popover_listbox || !GTK_IS_LIST_BOX(bm_popover_listbox)) return;
    GList* children = gtk_container_get_children(GTK_CONTAINER(bm_popover_listbox));
    for (GList* l = children; l != nullptr; l = l->next) {
        gtk_widget_destroy(GTK_WIDGET(l->data));
    }
    g_list_free(children);

    if (bookmark_list.empty()) {
        GtkWidget* empty_row = gtk_list_box_row_new();
        gtk_list_box_row_set_activatable(GTK_LIST_BOX_ROW(empty_row), FALSE);
        gtk_list_box_row_set_selectable(GTK_LIST_BOX_ROW(empty_row), FALSE);
        GtkWidget* lbl = gtk_label_new(tr_full("暂无书签", "暫無書籤", "No bookmarks yet"));
        gtk_container_set_border_width(GTK_CONTAINER(lbl), 18);
        gtk_container_add(GTK_CONTAINER(empty_row), lbl);
        gtk_widget_show_all(empty_row);
        gtk_list_box_insert(GTK_LIST_BOX(bm_popover_listbox), empty_row, -1);
        return;
    }

    for (int i = (int)bookmark_list.size() - 1; i >= 0; --i) {
        const auto& bm = bookmark_list[i];
        GtkWidget* row = gtk_list_box_row_new();
        gtk_list_box_row_set_activatable(GTK_LIST_BOX_ROW(row), FALSE);
        gtk_list_box_row_set_selectable(GTK_LIST_BOX_ROW(row), FALSE);

        GtkWidget* btn = gtk_button_new();
        gtk_button_set_relief(GTK_BUTTON(btn), GTK_RELIEF_NONE);
        gtk_widget_set_can_focus(btn, FALSE);
        gtk_style_context_add_class(gtk_widget_get_style_context(btn), "flat");
        gtk_widget_set_tooltip_text(btn, bm.url.c_str());

        GtkWidget* hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 8);
        gtk_box_pack_start(GTK_BOX(hbox), create_vector_icon(ICON_STAR_FILLED, 14), FALSE, FALSE, 0);
        std::string label_text = bm.title.empty() ? bm.url : bm.title;
        GtkWidget* lbl = gtk_label_new(label_text.c_str());
        gtk_label_set_xalign(GTK_LABEL(lbl), 0.0f);
        gtk_label_set_ellipsize(GTK_LABEL(lbl), PANGO_ELLIPSIZE_END);
        gtk_label_set_max_width_chars(GTK_LABEL(lbl), 32);
        gtk_box_pack_start(GTK_BOX(hbox), lbl, TRUE, TRUE, 0);
        gtk_container_add(GTK_CONTAINER(btn), hbox);

        std::string* url_copy = new std::string(bm.url);
        g_signal_connect_data(btn, "clicked", G_CALLBACK(+[](GtkWidget*, gpointer data) {
            std::string* u = static_cast<std::string*>(data);
            if (bm_popover) gtk_widget_hide(bm_popover);
            if (u && !u->empty()) {
                GtkWidget* cur_wv = get_current_webview();
                if (cur_wv) {
                    webkit_web_view_load_uri(WEBKIT_WEB_VIEW(cur_wv), u->c_str());
                } else {
                    add_tab(u->c_str());
                }
            }
        }), url_copy, +[](gpointer data, GClosure*) {
            delete static_cast<std::string*>(data);
        }, (GConnectFlags)0);

        gtk_container_add(GTK_CONTAINER(row), btn);
        gtk_widget_show_all(row);
        gtk_list_box_insert(GTK_LIST_BOX(bm_popover_listbox), row, -1);
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

void open_record_row_url(GtkTreeModel* model, GtkTreeIter* iter) {
    if (!model || !iter) return;
    gchar* url = nullptr;
    gtk_tree_model_get(model, iter, REC_URL, &url, -1);
    if (url && *url) {
        add_tab(url);
    }
    g_free(url);
}

gboolean on_record_button_press(GtkWidget* tree, GdkEventButton* event, gpointer) {
    if (event->type != GDK_BUTTON_PRESS || event->button != 3) return FALSE;

    GtkTreePath* path = nullptr;
    if (!gtk_tree_view_get_path_at_pos(GTK_TREE_VIEW(tree), (int)event->x, (int)event->y,
            &path, nullptr, nullptr, nullptr))
        return FALSE;

    GtkTreeModel* model = gtk_tree_view_get_model(GTK_TREE_VIEW(tree));
    GtkTreeIter iter;
    if (gtk_tree_model_get_iter(model, &iter, path)) {
        open_record_row_url(model, &iter);
    }
    gtk_tree_path_free(path);
    return TRUE;
}

void on_record_row_activated(GtkTreeView* tree, GtkTreePath* path, GtkTreeViewColumn*, gpointer) {
    GtkTreeModel* model = gtk_tree_view_get_model(tree);
    GtkTreeIter iter;
    if (model && path && gtk_tree_model_get_iter(model, &iter, path)) {
        open_record_row_url(model, &iter);
    }
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
    if (!log_mode) {
        g_signal_connect(tree, "button-press-event",
                         G_CALLBACK(on_record_button_press), nullptr);
        g_signal_connect(tree, "row-activated",
                         G_CALLBACK(on_record_row_activated), nullptr);
    }
    return tree;
}

void set_record_multi_mode(bool enabled) {
    record_multi_mode = enabled;
    active_download_multi_mode = enabled;
    const bool is_cookies = (pref_current_row == 8);
    if (record_multi_button) gtk_button_set_label(GTK_BUTTON(record_multi_button), enabled ? tr_full("退出多选", "退出多選", "Exit selection") : tr_full("多选", "多選", "Select"));
    if (record_select_all_button) gtk_widget_set_visible(record_select_all_button, enabled);
    if (record_invert_button) gtk_widget_set_visible(record_invert_button, enabled);
    if (record_delete_button) gtk_widget_set_visible(record_delete_button, enabled && !is_cookies);
    if (record_copy_button) gtk_widget_set_visible(record_copy_button, enabled);
    for (GtkTreeViewColumn* column : record_selection_columns)
        gtk_tree_view_column_set_visible(column, enabled);
    for (const auto& item : download_list) {
        if (!enabled) item->selected = false;
        refresh_download_row_ui(item);
    }
}

GtkListStore* current_pref_data_store() {
    if (pref_current_row == 5) return history_store;
    if (pref_current_row == 6) return bookmark_store;
    if (pref_current_row == 8) return cookie_store;
    return nullptr;
}

void on_record_multi(GtkWidget*, gpointer) { set_record_multi_mode(!record_multi_mode); }

void on_record_select_all(GtkWidget*, gpointer) {
    if (pref_current_row == 7) {
        for (const auto& item : download_list) {
            if (item->state != DownloadItem::State::InProgress) {
                item->selected = true;
                refresh_download_row_ui(item);
            }
        }
        return;
    }
    GtkListStore* store = current_pref_data_store();
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
    if (pref_current_row == 7) {
        for (const auto& item : download_list) {
            if (item->state != DownloadItem::State::InProgress) {
                item->selected = !item->selected;
                refresh_download_row_ui(item);
            }
        }
        return;
    }
    GtkListStore* store = current_pref_data_store();
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
    std::string copied;
    if (pref_current_row == 7) {
        for (auto it = download_list.rbegin(); it != download_list.rend(); ++it) {
            const auto& item = *it;
            if (item->state != DownloadItem::State::InProgress && item->selected) {
                copied += item->filename + "\t" + item->url + "\t" + item->destination_path + "\t" + item->time + "\n";
            }
        }
    } else {
        GtkListStore* store = current_pref_data_store();
        if (!store) return;
        GtkTreeIter iter;
        gboolean valid = gtk_tree_model_get_iter_first(GTK_TREE_MODEL(store), &iter);
        while (valid) {
            gboolean selected = FALSE;
            gtk_tree_model_get(GTK_TREE_MODEL(store), &iter, record_selected_column(GTK_TREE_MODEL(store)), &selected, -1);
            if (selected) {
                gchar *a = nullptr, *b = nullptr, *c = nullptr;
                if (pref_current_row == 5 || pref_current_row == 6) {
                    gtk_tree_model_get(GTK_TREE_MODEL(store), &iter, REC_TEXT, &a, REC_URL, &b, REC_TIME, &c, -1);
                    copied += (a ? a : "") + std::string("\t") + (b ? b : "") + "\t" + (c ? c : "") + "\n";
                } else if (pref_current_row == 8) {
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
    if (pref_current_row == 8) return;
    if (pref_current_row == 7) {
        download_list.erase(
            std::remove_if(download_list.begin(), download_list.end(), [](const std::shared_ptr<DownloadItem>& item) {
                return item->state != DownloadItem::State::InProgress && item->selected;
            }),
            download_list.end()
        );
        save_downloads();
        if (active_download_listbox) rebuild_download_list_ui(active_download_listbox, active_download_multi_mode);
        return;
    }
    GtkListStore* store = current_pref_data_store();
    if (!store) return;
    std::vector<int> indices;
    GtkTreeIter iter;
    gboolean valid = gtk_tree_model_get_iter_first(GTK_TREE_MODEL(store), &iter);
    while (valid) {
        gboolean selected = FALSE;
        gint index = -1;
        const bool log_mode = (pref_current_row == 9);
        gtk_tree_model_get(GTK_TREE_MODEL(store), &iter,
            log_mode ? 1 : REC_SELECTED, &selected,
            log_mode ? 2 : REC_INDEX, &index, -1);
        if (selected && index >= 0) indices.push_back(index);
        valid = gtk_tree_model_iter_next(GTK_TREE_MODEL(store), &iter);
    }
    std::sort(indices.rbegin(), indices.rend());
    for (int index : indices) {
        if (pref_current_row == 5 && index < (int)history_list.size()) history_list.erase(history_list.begin() + index);
        else if (pref_current_row == 6 && index < (int)bookmark_list.size()) bookmark_list.erase(bookmark_list.begin() + index);
    }
    if (pref_current_row == 5) { write_records(history_file, history_list); refresh_history_ui(); }
    else if (pref_current_row == 6) { write_records(bookmark_file, bookmark_list); refresh_bookmark_ui(); update_navigation_state(); }
}

void on_cookies_fetched(GObject* source_object, GAsyncResult* res, gpointer user_data) {
    GError* error = nullptr;
    GList* cookies = webkit_cookie_manager_get_all_cookies_finish(WEBKIT_COOKIE_MANAGER(source_object), res, &error);
    if (error) {
        g_error_free(error);
        if (cookies) g_list_free_full(cookies, (GDestroyNotify)soup_cookie_free);
        return;
    }
    if (!cookie_store || !GTK_IS_LIST_STORE(cookie_store)) {
        if (cookies) g_list_free_full(cookies, (GDestroyNotify)soup_cookie_free);
        return;
    }
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
}

void refresh_cookies_from_manager() {
    if (!cookie_manager) return;
    if (cookie_fetch_cancellable) {
        g_cancellable_cancel(cookie_fetch_cancellable);
        g_object_unref(cookie_fetch_cancellable);
    }
    cookie_fetch_cancellable = g_cancellable_new();
    webkit_cookie_manager_get_all_cookies(cookie_manager, cookie_fetch_cancellable, on_cookies_fetched, NULL);
}

void on_bookmark(GtkWidget* w, gpointer d) {
    GtkWidget* webview = get_current_webview();
    if (!webview) return;
    const gchar* uri = webkit_web_view_get_uri(WEBKIT_WEB_VIEW(webview));
    const gchar* title = webkit_web_view_get_title(WEBKIT_WEB_VIEW(webview));

    if (uri && *uri) {
        std::string t = (title && strlen(title) > 0) ? title : uri;
        t = sanitize_field(t);
        std::string time_buf = now_time_string();

        for (size_t i = 0; i < bookmark_list.size(); ++i) {
            if (bookmark_list[i].url == uri) {
                bookmark_list.erase(bookmark_list.begin() + i);
                write_records(bookmark_file, bookmark_list);
                refresh_bookmark_ui();
                update_navigation_state();
                update_status_text(tr_full("已取消收藏", "已取消收藏", "Bookmark removed"), TRUE);
                return;
            }
        }
        bookmark_list.push_back({time_buf, t, uri});
        write_records(bookmark_file, bookmark_list);
        refresh_bookmark_ui();
        update_navigation_state();
        update_status_text(tr_full("收藏成功", "收藏成功", "Bookmark added"), TRUE);
    }
}

void apply_page_zoom(GtkWidget* view);

const char* get_current_preferences_url() {
    if (pref_current_row == 5) return "about:history";
    if (pref_current_row == 6) return "about:bookmarks";
    if (pref_current_row == 7) return "about:downloads";
    if (pref_current_row == 8) return "about:cookies";
    return "about:preferences";
}

gboolean on_url_entry_focus_in(GtkWidget* widget, GdkEventFocus* event, gpointer user_data) {
    is_typing = TRUE;
    return FALSE;
}

gboolean on_url_entry_focus_out(GtkWidget* widget, GdkEventFocus* event, gpointer user_data) {
    is_typing = FALSE;
    GtkWidget* cur_page = get_current_page_widget();
    const char* ptype = get_page_type(cur_page);
    if (strcmp(ptype, "preferences") == 0) {
        if (url_entry && GTK_IS_ENTRY(url_entry)) gtk_entry_set_text(GTK_ENTRY(url_entry), get_current_preferences_url());
        return FALSE;
    }
    GtkWidget* webview = get_webview_from_page(cur_page);
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
    return trim_whitespace(text);
}

enum class ContextActionType {
    Cut,
    Copy,
    Paste,
    Delete,
    SelectAll,
    SearchSelected,
    OpenInNewTab,
    OpenInCurrentTab,
    DownloadUri,
    CopyUri,
    PageBack,
    PageForward,
    PageReloadOrStop,
    PageBookmark,
    PageSave,
    PageCopyUrl
};

struct UnifiedMenuItemData {
    GObject* target;
    bool is_entry;
    ContextActionType action;
    std::string uri_arg;
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
                    std::string target = get_search_url(selected_text);
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
            default:
                break;
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
                std::string script =
                    "(()=>{const s=String(window.getSelection()).trim();"
                    "if(s)location.href='" + get_search_url_prefix() + "'+encodeURIComponent(s);})();";
                webkit_web_view_evaluate_javascript(webview, script.c_str(), -1,
                    nullptr, nullptr, nullptr,
                    context_menu_run_js_finished, nullptr);
                break;
            }
            case ContextActionType::OpenInNewTab:
                if (!d->uri_arg.empty()) add_tab(d->uri_arg.c_str());
                break;
            case ContextActionType::OpenInCurrentTab:
                if (!d->uri_arg.empty()) webkit_web_view_load_uri(webview, d->uri_arg.c_str());
                break;
            case ContextActionType::DownloadUri:
                if (!d->uri_arg.empty()) {
                    WebKitDownload* dl = webkit_web_view_download_uri(webview, d->uri_arg.c_str());
                    if (dl) g_object_unref(dl);
                }
                break;
            case ContextActionType::CopyUri:
                if (!d->uri_arg.empty()) {
                    GtkClipboard* cb = gtk_clipboard_get(GDK_SELECTION_CLIPBOARD);
                    gtk_clipboard_set_text(cb, d->uri_arg.c_str(), (gint)d->uri_arg.size());
                    update_status_text(tr_full("已复制地址", "已複製位址", "Address copied"), TRUE);
                }
                break;
            case ContextActionType::PageBack:
                if (webkit_web_view_can_go_back(webview)) webkit_web_view_go_back(webview);
                break;
            case ContextActionType::PageForward:
                if (webkit_web_view_can_go_forward(webview)) webkit_web_view_go_forward(webview);
                break;
            case ContextActionType::PageReloadOrStop:
                webkit_web_view_reload(webview);
                break;
            case ContextActionType::PageBookmark:
                on_bookmark(nullptr, nullptr);
                break;
            case ContextActionType::PageSave: {
                const gchar* cur_uri = webkit_web_view_get_uri(webview);
                if (cur_uri && *cur_uri) {
                    WebKitDownload* dl = webkit_web_view_download_uri(webview, cur_uri);
                    if (dl) g_object_unref(dl);
                }
                break;
            }
            case ContextActionType::PageCopyUrl: {
                const gchar* cur_uri = webkit_web_view_get_uri(webview);
                if (cur_uri && *cur_uri) {
                    GtkClipboard* cb = gtk_clipboard_get(GDK_SELECTION_CLIPBOARD);
                    gtk_clipboard_set_text(cb, cur_uri, -1);
                    update_status_text(tr_full("已复制页面网址", "已複製頁面網址", "Page URL copied"), TRUE);
                }
                break;
            }
        }
    }
}

static void populate_unified_context_menu(
    GtkMenuShell* menu_shell,
    GtkEntry* entry,
    WebKitWebView* webview,
    WebKitHitTestResult* hit
) {
    if (!menu_shell || !GTK_IS_MENU_SHELL(menu_shell)) return;
    const bool is_entry = (entry != nullptr && GTK_IS_ENTRY(entry));
    const bool is_webview = (webview != nullptr && WEBKIT_IS_WEB_VIEW(webview));
    if (!is_entry && !is_webview) return;

    GObject* target_obj = is_entry ? G_OBJECT(entry) : G_OBJECT(webview);

    auto append_action_item = [&](const char* zh, const char* zhtw, const char* en,
                                   gboolean sensitive, ContextActionType action,
                                   const std::string& uri_arg = "") {
        GtkWidget* item = gtk_menu_item_new_with_label(tr_full(zh, zhtw, en));
        gtk_widget_set_sensitive(item, sensitive);

        auto* item_data = new UnifiedMenuItemData{target_obj, is_entry, action, uri_arg, TRUE};
        g_object_weak_ref(target_obj, on_unified_target_weak_notify, item_data);
        g_signal_connect_data(item, "activate",
            G_CALLBACK(on_unified_menu_item_activate),
            item_data,
            on_unified_menu_item_destroy,
            (GConnectFlags)0);
        gtk_menu_shell_append(menu_shell, item);
    };

    if (is_entry) {
        gint start_pos = 0, end_pos = 0;
        const gboolean has_selection = gtk_editable_get_selection_bounds(GTK_EDITABLE(entry), &start_pos, &end_pos) && (start_pos != end_pos);
        const gchar* current_text = gtk_entry_get_text(entry);
        const gboolean has_text = (current_text && *current_text);
        append_action_item("剪切", "剪下", "Cut", has_selection, ContextActionType::Cut);
        append_action_item("复制", "複製", "Copy", has_selection, ContextActionType::Copy);
        append_action_item("粘贴", "貼上", "Paste", TRUE, ContextActionType::Paste);
        append_action_item("删除", "刪除", "Delete", has_selection, ContextActionType::Delete);
        gtk_menu_shell_append(menu_shell, gtk_separator_menu_item_new());
        append_action_item("全选", "全選", "Select all", has_text, ContextActionType::SelectAll);
        append_action_item("搜索选中文本", "搜尋選取文字", "Search selected text", has_selection, ContextActionType::SearchSelected);
        return;
    }

    bool added_section = false;
    const gchar* link_uri = hit ? webkit_hit_test_result_get_link_uri(hit) : nullptr;
    const gchar* image_uri = hit ? webkit_hit_test_result_get_image_uri(hit) : nullptr;
    const gchar* media_uri = hit ? webkit_hit_test_result_get_media_uri(hit) : nullptr;
    const gboolean is_link = hit && (webkit_hit_test_result_context_is_link(hit) || (link_uri && *link_uri));
    const gboolean is_image = hit && (webkit_hit_test_result_context_is_image(hit) || (image_uri && *image_uri));
    const gboolean is_media = hit && (webkit_hit_test_result_context_is_media(hit) || (media_uri && *media_uri));
    const gboolean is_editable = hit && webkit_hit_test_result_context_is_editable(hit);
    const gboolean is_selection = hit && webkit_hit_test_result_context_is_selection(hit);

    if (is_link && link_uri && *link_uri) {
        append_action_item("在新标签页中打开链接", "在新分頁中開啟連結", "Open link in new tab", TRUE, ContextActionType::OpenInNewTab, link_uri);
        append_action_item("链接另存为...", "另存連結為...", "Save link as...", TRUE, ContextActionType::DownloadUri, link_uri);
        append_action_item("复制链接地址", "複製連結位址", "Copy link address", TRUE, ContextActionType::CopyUri, link_uri);
        added_section = true;
    }
    if (is_image && image_uri && *image_uri) {
        if (added_section) gtk_menu_shell_append(menu_shell, gtk_separator_menu_item_new());
        append_action_item("在新标签页中查看图片", "在新分頁中檢視圖片", "Open image in new tab", TRUE, ContextActionType::OpenInNewTab, image_uri);
        append_action_item("查看图片", "檢視圖片", "View image", TRUE, ContextActionType::OpenInCurrentTab, image_uri);
        append_action_item("保存图片...", "儲存圖片...", "Save image as...", TRUE, ContextActionType::DownloadUri, image_uri);
        append_action_item("复制图片地址", "複製圖片位址", "Copy image address", TRUE, ContextActionType::CopyUri, image_uri);
        added_section = true;
    }
    if (is_media && media_uri && *media_uri) {
        if (added_section) gtk_menu_shell_append(menu_shell, gtk_separator_menu_item_new());
        append_action_item("在新标签页中打开视频/媒体", "在新分頁中開啟影片/媒體", "Open media in new tab", TRUE, ContextActionType::OpenInNewTab, media_uri);
        append_action_item("查看视频/媒体", "檢視影片/媒體", "View media", TRUE, ContextActionType::OpenInCurrentTab, media_uri);
        append_action_item("保存视频/媒体...", "儲存影片/媒體...", "Save media as...", TRUE, ContextActionType::DownloadUri, media_uri);
        append_action_item("复制媒体地址", "複製媒體位址", "Copy media address", TRUE, ContextActionType::CopyUri, media_uri);
        added_section = true;
    }

    if (added_section) {
        gtk_menu_shell_append(menu_shell, gtk_separator_menu_item_new());
    }

    // 共用页面导航与操作（所有网页右键场景均保留）
    const gboolean can_back = webkit_web_view_can_go_back(webview);
    const gboolean can_fwd = webkit_web_view_can_go_forward(webview);
    append_action_item("后退", "後退", "Back", can_back, ContextActionType::PageBack);
    append_action_item("前进", "前進", "Forward", can_fwd, ContextActionType::PageForward);
    append_action_item("重新加载", "重新載入", "Reload", TRUE, ContextActionType::PageReloadOrStop);
    gtk_menu_shell_append(menu_shell, gtk_separator_menu_item_new());

    const gchar* cur_uri = webkit_web_view_get_uri(webview);
    bool bookmarked = false;
    if (cur_uri && *cur_uri) {
        for (const auto& bm : bookmark_list) {
            if (bm.url == cur_uri) { bookmarked = true; break; }
        }
    }
    if (bookmarked) {
        append_action_item("取消收藏此页", "取消收藏此頁", "Remove bookmark", TRUE, ContextActionType::PageBookmark);
    } else {
        append_action_item("收藏此页", "收藏此頁", "Bookmark this page", TRUE, ContextActionType::PageBookmark);
    }
    append_action_item("另存页面为...", "另存頁面為...", "Save page as...", (cur_uri && *cur_uri), ContextActionType::PageSave);
    append_action_item("复制当前页面网址", "複製目前頁面網址", "Copy page URL", (cur_uri && *cur_uri), ContextActionType::PageCopyUrl);

    gtk_menu_shell_append(menu_shell, gtk_separator_menu_item_new());
    if (is_editable) {
        append_action_item("剪切", "剪下", "Cut", TRUE, ContextActionType::Cut);
        append_action_item("复制", "複製", "Copy", TRUE, ContextActionType::Copy);
        append_action_item("粘贴", "貼上", "Paste", TRUE, ContextActionType::Paste);
        append_action_item("删除", "刪除", "Delete", TRUE, ContextActionType::Delete);
        gtk_menu_shell_append(menu_shell, gtk_separator_menu_item_new());
    } else {
        append_action_item("复制", "複製", "Copy", is_selection, ContextActionType::Copy);
    }
    append_action_item("全选", "全選", "Select all", TRUE, ContextActionType::SelectAll);
    append_action_item("搜索选中文本", "搜尋選取文字", "Search selected text", is_selection, ContextActionType::SearchSelected);
}

void on_url_entry_populate_popup(GtkEntry* entry, GtkWidget* popup, gpointer) {
    if (!entry || !GTK_IS_ENTRY(entry) || !popup || !GTK_IS_MENU_SHELL(popup)) return;

    GList* children = gtk_container_get_children(GTK_CONTAINER(popup));
    for (GList* l = children; l != nullptr; l = l->next) {
        gtk_widget_destroy(GTK_WIDGET(l->data));
    }
    g_list_free(children);

    populate_unified_context_menu(GTK_MENU_SHELL(popup), entry, nullptr, nullptr);
    gtk_widget_show_all(popup);
}

void on_load_changed(WebKitWebView* web_view, WebKitLoadEvent load_event, gpointer user_data) {
    gboolean is_foreground = (GTK_WIDGET(web_view) == get_current_webview());
    if (is_foreground) update_navigation_state();

    if (load_event == WEBKIT_LOAD_STARTED) {
        g_object_set_data(G_OBJECT(web_view), "wb_load_committed", GINT_TO_POINTER(0));
        set_webview_visually_finished(web_view, false);
        set_webview_max_progress(web_view, 0.12);
        if (is_foreground) {
            update_status_text(tr_full("正在连接...", "正在連線...", "Connecting..."), FALSE);
            gtk_widget_show(progress_bar);
            gtk_progress_bar_set_fraction(GTK_PROGRESS_BAR(progress_bar), 0.12);
        }
    } else if (load_event == WEBKIT_LOAD_REDIRECTED) {
        if (is_foreground) update_status_text(tr_full("正在重定向...", "正在重新導向...", "Redirecting..."), FALSE);
    } else if (load_event == WEBKIT_LOAD_COMMITTED) {
        g_object_set_data(G_OBJECT(web_view), "wb_load_committed", GINT_TO_POINTER(1));
        set_webview_max_progress(web_view, std::max(get_webview_max_progress(web_view), 0.25));
        if (is_foreground) {
            update_status_text(tr_full("等待响应...", "等待回應...", "Waiting for response..."), FALSE);
            if (!is_typing && url_entry && GTK_IS_ENTRY(url_entry)) {
                const gchar* uri = webkit_web_view_get_uri(web_view);
                gtk_entry_set_text(GTK_ENTRY(url_entry), uri ? uri : "");
            }
        }
    } else if (load_event == WEBKIT_LOAD_FINISHED) {
        set_webview_visually_finished(web_view, true);
        set_webview_max_progress(web_view, 1.0);
        run_selection_toolbar_script(web_view);
        if (is_foreground) {
            gtk_progress_bar_set_fraction(GTK_PROGRESS_BAR(progress_bar), 1.0);
            g_timeout_add(220, hide_progress_if_idle, NULL);
            update_status_text(tr_full("完成", "完成", "Done"), TRUE);
        }

        const gchar* uri = webkit_web_view_get_uri(web_view);
        const gchar* title = webkit_web_view_get_title(web_view);
        save_history(uri, title);

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
        return TRUE;
    }
    if (error && (g_error_matches(error, WEBKIT_NETWORK_ERROR, WEBKIT_NETWORK_ERROR_CANCELLED) ||
                  g_error_matches(error, G_IO_ERROR, G_IO_ERROR_CANCELLED))) {
        if (GTK_WIDGET(web_view) == get_current_webview() && progress_bar && !webkit_web_view_is_loading(web_view)) {
            gtk_widget_hide(progress_bar);
        }
        return TRUE;
    }
    if (error && error->message && (strstr(error->message, "cancelled") || strstr(error->message, "Cancelled"))) {
        if (GTK_WIDGET(web_view) == get_current_webview() && progress_bar && !webkit_web_view_is_loading(web_view)) {
            gtk_widget_hide(progress_bar);
        }
        return TRUE;
    }

    const gchar* uri = failing_uri ? failing_uri : "";
    const std::string reason = error_reason(error);
    const std::string escaped_reason = html_escape(reason.c_str());
    const std::string escaped_uri = html_escape(uri);
    const std::string escaped_detail = html_escape(error && error->message ? error->message : "Unknown error");
    const std::string code_text = error_code_name(error) + (error ? " (" + std::to_string(error->code) + ")" : "");
    const std::string escaped_code = html_escape(code_text.c_str());
    const bool dark = ui_theme == "dark";
    const std::string bg = dark ? "#1c1b22" : "#f9f9fb";
    const std::string fg = dark ? "#fbfbfe" : "#15141a";
    const std::string muted = dark ? "#cfcfd8" : "#5b5b66";
    const std::string panel = dark ? "#2b2a33" : "#f0f0f4";
    std::string html =
        "<!doctype html><html><head><meta charset='utf-8'><meta name='viewport' content='width=device-width,initial-scale=1'>"
        "<title>" + std::string(tr_full("网页无法访问", "網頁無法存取", "This site can’t be reached")) + "</title><style>"
        "*{box-sizing:border-box}html,body{min-height:100%;margin:0;font-family:Arial,'Noto Sans CJK SC',sans-serif;background:" + bg + ";color:" + fg + ";}"
        "body{padding:clamp(18px,5vw,42px)}.wrap{width:100%;max-width:780px;margin:clamp(12px,7vh,64px) auto 32px;}"
        ".x{color:#e53935;font-size:64px;font-weight:700;line-height:1;margin:0 0 18px 0;}"
        "h1{font-size:clamp(24px,4vw,32px);line-height:1.25;margin:0 0 14px;font-weight:650;}"
        "p{font-size:16px;line-height:1.75;color:" + muted + ";margin:0 0 14px;overflow-wrap:anywhere;word-break:break-word;}"
        ".url{display:block;margin:18px 0;padding:12px 14px;border-radius:9px;background:" + panel + ";overflow-wrap:anywhere;word-break:break-all;}"
        "a{color:#0060df;text-decoration:none}a:hover{text-decoration:underline}"
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

    if (GTK_WIDGET(web_view) == get_current_webview()) {
        gtk_widget_hide(progress_bar);
        update_status_text(tr_full("网页无法访问", "網頁無法存取", "Page unavailable"), FALSE);
    }
    webkit_web_view_load_alternate_html(web_view, html.c_str(), uri, NULL);
    return TRUE;
}

void on_web_process_terminated(WebKitWebView* web_view, WebKitWebProcessTerminationReason reason, gpointer user_data) {
    if (!web_view || reason == WEBKIT_WEB_PROCESS_TERMINATED_BY_API) return;
    const gchar* uri = webkit_web_view_get_uri(web_view);
    if (!uri || !*uri) return;

    auto* state = static_cast<WebProcessCrashState*>(g_object_get_data(G_OBJECT(web_view), "wb_crash_state"));
    if (!state) {
        state = new WebProcessCrashState();
        g_object_set_data_full(G_OBJECT(web_view), "wb_crash_state", state, [](gpointer p) {
            delete static_cast<WebProcessCrashState*>(p);
        });
    }

    const gint64 now_us = g_get_monotonic_time();
    if (state->last_uri == uri && (now_us - state->last_time_us) < 30 * G_USEC_PER_SEC) {
        state->count++;
    } else {
        state->last_uri = uri;
        state->count = 1;
    }
    state->last_time_us = now_us;

    if (state->count <= 1) {
        webkit_web_view_load_uri(web_view, uri);
        return;
    }

    if (GTK_WIDGET(web_view) == get_current_webview()) {
        if (progress_bar) gtk_widget_hide(progress_bar);
        update_status_text(tr_full("页面渲染异常", "頁面算繪異常", "Page process crashed"), FALSE);
    }
    const bool dark = ui_theme == "dark";
    const std::string bg = dark ? "#1c1b22" : "#f9f9fb";
    const std::string card = dark ? "#23222b" : "#ffffff";
    const std::string border = dark ? "#383745" : "#e0e0e6";
    const std::string fg = dark ? "#fbfbfe" : "#15141a";
    const std::string muted = dark ? "#cfcfd8" : "#5b5b66";
    const std::string escaped_uri = html_escape(uri);
    std::string html =
        "<!doctype html><html><head><meta charset='utf-8'><meta name='viewport' content='width=device-width,initial-scale=1'>"
        "<title>" + std::string(tr_full("页面崩溃", "頁面崩潰", "Page crashed")) + "</title><style>"
        "body{margin:0;padding:36px 24px;font-family:Arial,'Noto Sans CJK SC',sans-serif;background:" + bg + ";color:" + fg + ";}"
        ".wrap{max-width:680px;margin:40px auto;background:" + card + ";border:1px solid " + border + ";border-radius:12px;padding:28px;}"
        "h1{margin:0 0 12px;font-size:24px;}p{color:" + muted + ";line-height:1.7;margin:0 0 18px;overflow-wrap:anywhere;}"
        "button{background:#0060df;color:#ffffff;border:0;border-radius:6px;padding:10px 18px;font-size:14px;cursor:pointer;}"
        "</style></head><body><div class='wrap'>"
        "<h1>" + std::string(tr_full("页面渲染进程已停止", "頁面算繪程序已停止", "The page process stopped")) + "</h1>"
        "<p>" + std::string(tr_full("为避免无限刷新循环，浏览器已暂停自动重新加载此页面。", "為避免無限重新整理循環，瀏覽器已暫停自動重新載入此頁面。", "To avoid an infinite reload loop, automatic reloading has been paused for this page.")) + "</p>"
        "<p>" + escaped_uri + "</p>"
        "<button onclick=\"location.href='" + escaped_uri + "'\">" + std::string(tr_full("手动重新加载", "手動重新載入", "Reload manually")) + "</button>"
        "</div></body></html>";
    webkit_web_view_load_alternate_html(web_view, html.c_str(), uri, NULL);
}

void on_url_activate(GtkWidget* widget, gpointer data) {
    if (!url_entry || !GTK_IS_ENTRY(url_entry)) return;
    std::string raw = trim_whitespace(gtk_entry_get_text(GTK_ENTRY(url_entry)));
    if (raw == "about:preferences" || raw == "about:settings") {
        is_typing = FALSE;
        open_preferences_tab("general");
        return;
    }
    if (raw == "about:about") {
        is_typing = FALSE;
        open_preferences_tab("about");
        return;
    }
    if (raw == "about:library" || raw == "about:history") {
        is_typing = FALSE;
        open_preferences_tab("history");
        return;
    }
    if (raw == "about:bookmarks") {
        is_typing = FALSE;
        open_preferences_tab("bookmarks");
        return;
    }
    if (raw == "about:downloads") {
        is_typing = FALSE;
        open_preferences_tab("downloads_list");
        return;
    }
    if (raw == "about:cookies") {
        is_typing = FALSE;
        open_preferences_tab("cookies");
        return;
    }
    if (raw == "about:logs") {
        is_typing = FALSE;
        open_preferences_tab("logs");
        return;
    }

    std::string target = resolve_input(raw.c_str());
    is_typing = FALSE;
    GtkWidget* webview = get_current_webview();
    if (webview) {
        webkit_web_view_load_uri(WEBKIT_WEB_VIEW(webview), target.c_str());
    } else {
        add_tab(target.c_str());
    }
}

void apply_page_zoom(GtkWidget* view) {
    if (!view || !WEBKIT_IS_WEB_VIEW(view)) return;
    const gint width = gtk_widget_get_allocated_width(view);
    if (width <= 0) return;
    double zoom = (double)width / 1366.0;
    if (zoom < 0.3) zoom = 0.3;
    if (zoom > 1.0) zoom = 1.0;
    zoom *= (page_zoom_percent / 50.0) * 0.72;
    if (zoom < 0.15) zoom = 0.15;
    if (zoom > 2.0) zoom = 2.0;
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

void apply_web_settings_to_all_tabs(bool reload_for_lang) {
    if (!notebook || !GTK_IS_NOTEBOOK(notebook)) return;
    for (int i = 0; i < gtk_notebook_get_n_pages(GTK_NOTEBOOK(notebook)); ++i) {
        GtkWidget* page = gtk_notebook_get_nth_page(GTK_NOTEBOOK(notebook), i);
        GtkWidget* view = get_webview_from_page(page);
        if (view) {
            WebKitSettings* settings = webkit_web_view_get_settings(WEBKIT_WEB_VIEW(view));
            webkit_settings_set_enable_javascript(settings, javascript_enabled);
            webkit_settings_set_enable_webgl(settings, webgl_enabled);
            webkit_settings_set_media_playback_requires_user_gesture(settings, media_requires_gesture);
            if (reload_for_lang) webkit_web_view_reload(WEBKIT_WEB_VIEW(view));
        }
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

    std::string js = "window.__wbSearchUrl='" + get_search_url_prefix() + "';" + R"JS((() => {
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
        if (text) location.href=(window.__wbSearchUrl||'https://www.google.com/search?q=')+encodeURIComponent(text);
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

void update_search_engine_in_all_tabs() {
    if (!notebook || !GTK_IS_NOTEBOOK(notebook)) return;
    std::string js = "window.__wbSearchUrl='" + get_search_url_prefix() + "';";
    for (int i = 0; i < gtk_notebook_get_n_pages(GTK_NOTEBOOK(notebook)); ++i) {
        GtkWidget* page = gtk_notebook_get_nth_page(GTK_NOTEBOOK(notebook), i);
        GtkWidget* view = get_webview_from_page(page);
        if (view && WEBKIT_IS_WEB_VIEW(view)) {
            webkit_web_view_evaluate_javascript(WEBKIT_WEB_VIEW(view), js.c_str(), -1,
                nullptr, nullptr, nullptr,
                context_menu_run_js_finished, nullptr);
        }
    }
}

gboolean on_webview_context_menu(WebKitWebView* view, WebKitContextMenu*,
        GdkEvent* event, WebKitHitTestResult* hit_test_result, gpointer) {
    if (!view || !WEBKIT_IS_WEB_VIEW(view)) return TRUE;
    GtkWidget* menu = gtk_menu_new();
    g_signal_connect(menu, "selection-done", G_CALLBACK(gtk_widget_destroy), NULL);
    populate_unified_context_menu(GTK_MENU_SHELL(menu), nullptr, view, hit_test_result);
    gtk_widget_show_all(menu);
    gtk_menu_popup_at_pointer(GTK_MENU(menu), event);
    return TRUE;
}

void close_tab_at_index(int page_idx) {
    if (!notebook || !GTK_IS_NOTEBOOK(notebook)) return;
    if (page_idx < 0 || page_idx >= gtk_notebook_get_n_pages(GTK_NOTEBOOK(notebook))) return;
    gtk_notebook_remove_page(GTK_NOTEBOOK(notebook), page_idx);
    if (gtk_notebook_get_n_pages(GTK_NOTEBOOK(notebook)) == 0) {
        add_tab(get_home_url().c_str());
    } else {
        sync_tab_bar();
        update_navigation_state();
    }
}

void sync_tab_bar() {
    if (!tab_bar_box || !notebook || !GTK_IS_NOTEBOOK(notebook)) return;
    GList* children = gtk_container_get_children(GTK_CONTAINER(tab_bar_box));
    for (GList* l = children; l != nullptr; l = l->next) {
        gtk_widget_destroy(GTK_WIDGET(l->data));
    }
    g_list_free(children);

    const int n_pages = gtk_notebook_get_n_pages(GTK_NOTEBOOK(notebook));
    const int current_page = gtk_notebook_get_current_page(GTK_NOTEBOOK(notebook));

    for (int i = 0; i < n_pages; ++i) {
        GtkWidget* page = gtk_notebook_get_nth_page(GTK_NOTEBOOK(notebook), i);
        const char* ptype = get_page_type(page);
        std::string display_title;

        bool is_pref = (strcmp(ptype, "preferences") == 0);
        if (is_pref) {
            if (pref_current_row == 5) display_title = tr_full("历史记录", "歷史記錄", "History");
            else if (pref_current_row == 6) display_title = tr_full("书签管理", "書籤管理", "Bookmarks");
            else if (pref_current_row == 7) display_title = tr_full("下载管理", "下載管理", "Downloads");
            else if (pref_current_row == 8) display_title = "Cookies";
            else if (pref_current_row == 9) display_title = tr_full("关于 WebBrowser", "關於 WebBrowser", "About WebBrowser");
            else display_title = tr_full("设置", "設定", "Settings");
        } else {
            GtkWidget* wv = get_webview_from_page(page);
            const gchar* title = (wv && WEBKIT_IS_WEB_VIEW(wv)) ? webkit_web_view_get_title(WEBKIT_WEB_VIEW(wv)) : nullptr;
            display_title = (title && *title) ? title : tr_full("新标签页", "新分頁", "New tab");
        }

        GtkWidget* pill = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 2);
        GtkStyleContext* sc = gtk_widget_get_style_context(pill);
        gtk_style_context_add_class(sc, i == current_page ? "tab-pill-active" : "tab-pill");

        GtkWidget* tab_btn = gtk_button_new();
        gtk_widget_set_can_focus(tab_btn, FALSE);
        gtk_button_set_relief(GTK_BUTTON(tab_btn), GTK_RELIEF_NONE);
        gtk_style_context_add_class(gtk_widget_get_style_context(tab_btn), "flat");
        GtkWidget* tab_inner = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
        if (is_pref) {
            if (pref_current_row == 9) {
                GdkPixbuf* logo_pb = gdk_pixbuf_new_from_file_at_scale(app_icon_path.c_str(), 14, 14, TRUE, NULL);
                if (logo_pb) {
                    GtkWidget* logo_img = gtk_image_new_from_pixbuf(logo_pb);
                    g_object_unref(logo_pb);
                    gtk_widget_set_halign(logo_img, GTK_ALIGN_CENTER);
                    gtk_widget_set_valign(logo_img, GTK_ALIGN_CENTER);
                    gtk_box_pack_start(GTK_BOX(tab_inner), logo_img, FALSE, FALSE, 0);
                } else {
                    gtk_box_pack_start(GTK_BOX(tab_inner), create_vector_icon(ICON_APP_LOGO, 14), FALSE, FALSE, 0);
                }
            } else {
                IconType picon = ICON_SETTINGS;
                if (pref_current_row == 5) picon = ICON_HISTORY;
                else if (pref_current_row == 6) picon = ICON_BOOKMARKS;
                else if (pref_current_row == 7) picon = ICON_DOWNLOAD;
                else if (pref_current_row == 8) picon = ICON_COOKIE;
                gtk_box_pack_start(GTK_BOX(tab_inner), create_vector_icon(picon, 14), FALSE, FALSE, 0);
            }
        } else {
            GtkWidget* wv = get_webview_from_page(page);
            cairo_surface_t* fav = (wv && WEBKIT_IS_WEB_VIEW(wv)) ? webkit_web_view_get_favicon(WEBKIT_WEB_VIEW(wv)) : nullptr;
            if (fav) {
                gtk_box_pack_start(GTK_BOX(tab_inner), create_favicon_widget(fav, 14), FALSE, FALSE, 0);
            } else {
                gtk_box_pack_start(GTK_BOX(tab_inner), create_vector_icon(ICON_GLOBE, 14), FALSE, FALSE, 0);
            }
        }
        GtkWidget* tab_lbl = gtk_label_new(display_title.c_str());
        gtk_label_set_ellipsize(GTK_LABEL(tab_lbl), PANGO_ELLIPSIZE_END);
        gtk_label_set_width_chars(GTK_LABEL(tab_lbl), 8);
        gtk_label_set_max_width_chars(GTK_LABEL(tab_lbl), 20);
        gtk_label_set_xalign(GTK_LABEL(tab_lbl), 0.0f);
        gtk_box_pack_start(GTK_BOX(tab_inner), tab_lbl, TRUE, TRUE, 0);
        gtk_container_add(GTK_CONTAINER(tab_btn), tab_inner);
        g_signal_connect(tab_btn, "clicked", G_CALLBACK(+[](GtkWidget*, gpointer data) {
            int idx = GPOINTER_TO_INT(data);
            if (notebook && GTK_IS_NOTEBOOK(notebook)) gtk_notebook_set_current_page(GTK_NOTEBOOK(notebook), idx);
        }), GINT_TO_POINTER(i));

        GtkWidget* close_btn = gtk_button_new();
        gtk_widget_set_can_focus(close_btn, FALSE);
        gtk_container_add(GTK_CONTAINER(close_btn), create_vector_icon(ICON_CLOSE, 12));
        gtk_button_set_relief(GTK_BUTTON(close_btn), GTK_RELIEF_NONE);
        gtk_style_context_add_class(gtk_widget_get_style_context(close_btn), "flat");
        gtk_widget_set_tooltip_text(close_btn, tr_full("关闭标签页", "關閉分頁", "Close tab"));
        g_signal_connect(close_btn, "clicked", G_CALLBACK(+[](GtkWidget*, gpointer data) {
            close_tab_at_index(GPOINTER_TO_INT(data));
        }), GINT_TO_POINTER(i));

        gtk_box_pack_start(GTK_BOX(pill), tab_btn, FALSE, FALSE, 0);
        gtk_box_pack_start(GTK_BOX(pill), close_btn, FALSE, FALSE, 0);
        gtk_box_pack_start(GTK_BOX(tab_bar_box), pill, FALSE, FALSE, 0);
    }

    GtkWidget* add_btn = gtk_button_new();
    gtk_widget_set_can_focus(add_btn, FALSE);
    gtk_container_add(GTK_CONTAINER(add_btn), create_vector_icon(ICON_PLUS, 14));
    gtk_button_set_relief(GTK_BUTTON(add_btn), GTK_RELIEF_NONE);
    gtk_style_context_add_class(gtk_widget_get_style_context(add_btn), "flat");
    gtk_widget_set_tooltip_text(add_btn, tr_full("新建标签页", "新增分頁", "New tab"));
    g_signal_connect(add_btn, "clicked", G_CALLBACK(+[](GtkWidget*, gpointer) {
        add_tab(get_home_url().c_str());
    }), NULL);
    gtk_box_pack_start(GTK_BOX(tab_bar_box), add_btn, FALSE, FALSE, 0);

    gtk_widget_show_all(tab_bar_box);
}

void add_tab(const char* url) {
    if (!notebook || !GTK_IS_NOTEBOOK(notebook) || !web_context) return;
    GtkWidget* webview = GTK_WIDGET(g_object_new(
        WEBKIT_TYPE_WEB_VIEW,
        "web-context", web_context,
        "user-content-manager", user_content_manager,
        NULL));
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
    g_signal_connect(webview, "notify::is-loading", G_CALLBACK(on_is_loading_changed), NULL);
    g_signal_connect(webview, "decide-policy", G_CALLBACK(on_decide_policy), NULL);
    g_signal_connect(webview, "context-menu", G_CALLBACK(on_webview_context_menu), NULL);

    g_signal_connect(webview, "notify::title", G_CALLBACK(+[](GtkWidget*, GParamSpec*, gpointer){
        sync_tab_bar();
    }), NULL);
    g_signal_connect(webview, "notify::favicon", G_CALLBACK(+[](GtkWidget*, GParamSpec*, gpointer){
        sync_tab_bar();
    }), NULL);

    GtkWidget* vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    g_object_set_data(G_OBJECT(vbox), "wb_page_type", (gpointer)"web");
    gtk_box_pack_start(GTK_BOX(vbox), webview, TRUE, TRUE, 0);
    gtk_widget_show_all(vbox);

    int page = gtk_notebook_append_page(GTK_NOTEBOOK(notebook), vbox, NULL);
    gtk_widget_show_all(vbox);
    gtk_notebook_set_current_page(GTK_NOTEBOOK(notebook), page);
    std::string target_url = (url && *url) ? url : get_home_url();
    webkit_web_view_load_uri(WEBKIT_WEB_VIEW(webview), target_url.c_str());
    sync_tab_bar();
    update_navigation_state();
}

static std::string get_pref_ad_buffer_text() {
    if (!pref_ad_buf || !GTK_IS_TEXT_BUFFER(pref_ad_buf)) return adblock_rules_text;
    GtkTextIter start, end;
    gtk_text_buffer_get_bounds(pref_ad_buf, &start, &end);
    gchar* txt = gtk_text_buffer_get_text(pref_ad_buf, &start, &end, FALSE);
    std::string res = txt ? txt : "";
    g_free(txt);
    return res;
}

void update_pref_action_bars_visibility() {
    if (pref_gen_action_bar) {
        bool dirty = false;
        if (pref_theme_combo) {
            const gchar* id = gtk_combo_box_get_active_id(GTK_COMBO_BOX(pref_theme_combo));
            if (id && ui_theme != id) dirty = true;
        }
        if (pref_lang_combo) {
            const gchar* id = gtk_combo_box_get_active_id(GTK_COMBO_BOX(pref_lang_combo));
            if (id && ui_language != id) dirty = true;
        }
        if (pref_zoom_scale) {
            double val = gtk_range_get_value(GTK_RANGE(pref_zoom_scale));
            if (std::lround(val) != std::lround(page_zoom_percent)) dirty = true;
        }
        gtk_widget_set_visible(pref_gen_action_bar, dirty);
    }
    if (pref_srch_action_bar) {
        bool dirty = false;
        if (pref_search_combo) {
            const gchar* id = gtk_combo_box_get_active_id(GTK_COMBO_BOX(pref_search_combo));
            if (id && search_engine != id) dirty = true;
        }
        gtk_widget_set_visible(pref_srch_action_bar, dirty);
    }
    if (pref_dl_action_bar) {
        bool dirty = false;
        if (pref_dl_entry) {
            std::string val = trim_whitespace(gtk_entry_get_text(GTK_ENTRY(pref_dl_entry)));
            if (val != download_dir) dirty = true;
        }
        gtk_widget_set_visible(pref_dl_action_bar, dirty);
    }
    if (pref_priv_action_bar) {
        bool dirty = false;
        if (pref_cookie_combo) {
            const gchar* id = gtk_combo_box_get_active_id(GTK_COMBO_BOX(pref_cookie_combo));
            if (id && cookie_policy != id) dirty = true;
        }
        if (pref_media_check && gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(pref_media_check)) != media_requires_gesture) dirty = true;
        if (pref_tls_check && gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(pref_tls_check)) != ignore_tls_errors) dirty = true;
        if (pref_ad_check && gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(pref_ad_check)) != adblock_enabled) dirty = true;
        if (pref_ad_buf && get_pref_ad_buffer_text() != adblock_rules_text) dirty = true;
        gtk_widget_set_visible(pref_priv_action_bar, dirty);
    }
    if (pref_dev_action_bar) {
        bool dirty = false;
        if (pref_js_check && gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(pref_js_check)) != javascript_enabled) dirty = true;
        if (pref_webgl_check && gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(pref_webgl_check)) != webgl_enabled) dirty = true;
        gtk_widget_set_visible(pref_dev_action_bar, dirty);
    }
}

void reset_all_pref_drafts() {
    pref_syncing_ui = true;
    if (pref_theme_combo) gtk_combo_box_set_active_id(GTK_COMBO_BOX(pref_theme_combo), ui_theme.c_str());
    if (pref_lang_combo) gtk_combo_box_set_active_id(GTK_COMBO_BOX(pref_lang_combo), ui_language.c_str());
    if (pref_zoom_scale) gtk_range_set_value(GTK_RANGE(pref_zoom_scale), page_zoom_percent);
    if (pref_search_combo) gtk_combo_box_set_active_id(GTK_COMBO_BOX(pref_search_combo), search_engine.c_str());
    if (pref_dl_entry) gtk_entry_set_text(GTK_ENTRY(pref_dl_entry), download_dir.c_str());
    if (pref_cookie_combo) gtk_combo_box_set_active_id(GTK_COMBO_BOX(pref_cookie_combo), cookie_policy.c_str());
    if (pref_media_check) gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(pref_media_check), media_requires_gesture);
    if (pref_tls_check) gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(pref_tls_check), ignore_tls_errors);
    if (pref_ad_check) gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(pref_ad_check), adblock_enabled);
    if (pref_ad_buf) gtk_text_buffer_set_text(pref_ad_buf, adblock_rules_text.c_str(), -1);
    if (pref_js_check) gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(pref_js_check), javascript_enabled);
    if (pref_webgl_check) gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(pref_webgl_check), webgl_enabled);
    pref_syncing_ui = false;
    update_pref_action_bars_visibility();
}

static GtkWidget* create_pref_action_bar(GCallback on_apply_cb) {
    GtkWidget* bar = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
    gtk_widget_set_halign(bar, GTK_ALIGN_END);
    gtk_widget_set_margin_top(bar, 6);
    GtkWidget* cancel_btn = gtk_button_new_with_label(tr_full("取消", "取消", "Cancel"));
    GtkWidget* apply_btn = gtk_button_new_with_label(tr_full("应用", "套用", "Apply"));
    gtk_widget_set_can_focus(cancel_btn, FALSE);
    gtk_widget_set_can_focus(apply_btn, FALSE);
    gtk_style_context_add_class(gtk_widget_get_style_context(apply_btn), "suggested-action");
    g_signal_connect(cancel_btn, "clicked", G_CALLBACK(+[](GtkWidget*, gpointer) {
        reset_all_pref_drafts();
    }), NULL);
    g_signal_connect(apply_btn, "clicked", on_apply_cb, NULL);
    gtk_box_pack_start(GTK_BOX(bar), cancel_btn, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(bar), apply_btn, FALSE, FALSE, 0);
    gtk_widget_show_all(bar);
    gtk_widget_set_no_show_all(bar, TRUE);
    gtk_widget_hide(bar);
    return bar;
}

void on_switch_page(GtkNotebook* nb, GtkWidget* page, guint page_num, gpointer data) {
    is_typing = FALSE;
    const char* ptype = get_page_type(page);
    if (strcmp(ptype, "preferences") != 0) {
        reset_all_pref_drafts();
    }
    if (strcmp(ptype, "preferences") == 0) {
        if (url_entry && GTK_IS_ENTRY(url_entry)) gtk_entry_set_text(GTK_ENTRY(url_entry), get_current_preferences_url());
        if (progress_bar) gtk_widget_hide(progress_bar);
        if (status_overlay) gtk_widget_hide(status_overlay);
        sync_tab_bar();
        update_navigation_state();
        return;
    }

    GtkWidget* webview = get_webview_from_page(page);
    if (webview) {
        const gchar* uri = webkit_web_view_get_uri(WEBKIT_WEB_VIEW(webview));
        if (url_entry && GTK_IS_ENTRY(url_entry))
            gtk_entry_set_text(GTK_ENTRY(url_entry), uri ? uri : "");
        apply_page_zoom(webview);

        if (webkit_web_view_is_loading(WEBKIT_WEB_VIEW(webview)) &&
            !is_webview_visually_finished(WEBKIT_WEB_VIEW(webview))) {
            double p = std::max(get_webview_max_progress(WEBKIT_WEB_VIEW(webview)),
                webkit_web_view_get_estimated_load_progress(WEBKIT_WEB_VIEW(webview)));
            gtk_widget_show(progress_bar);
            gtk_progress_bar_set_fraction(GTK_PROGRESS_BAR(progress_bar), p);
            update_status_text(tr_full("正在加载...", "正在載入...", "Loading..."), FALSE);
            if (GPOINTER_TO_INT(g_object_get_data(G_OBJECT(webview), "wb_load_committed")) != 0) {
                }
        } else {
            gtk_widget_hide(progress_bar);
            if (status_overlay) gtk_widget_hide(status_overlay);
        }
    }
    sync_tab_bar();
    update_navigation_state();
}

void on_back(GtkWidget* w, gpointer d) {
    GtkWidget* cur_page = get_current_page_widget();
    const char* ptype = get_page_type(cur_page);
    if (strcmp(ptype, "preferences") == 0 && preferences_sidebar && !pref_back_stack.empty()) {
        int prev = pref_back_stack.back();
        pref_back_stack.pop_back();
        if (pref_current_row >= 0) pref_forward_stack.push_back(pref_current_row);
        pref_navigating_history = true;
        GtkListBoxRow* r = gtk_list_box_get_row_at_index(GTK_LIST_BOX(preferences_sidebar), prev);
        if (r) gtk_list_box_select_row(GTK_LIST_BOX(preferences_sidebar), r);
        pref_navigating_history = false;
        update_navigation_state();
        return;
    }

    GtkWidget* wv = get_webview_from_page(cur_page);
    if (wv) webkit_web_view_go_back(WEBKIT_WEB_VIEW(wv));
}

void on_forward(GtkWidget* w, gpointer d) {
    GtkWidget* cur_page = get_current_page_widget();
    const char* ptype = get_page_type(cur_page);
    if (strcmp(ptype, "preferences") == 0 && preferences_sidebar && !pref_forward_stack.empty()) {
        int next = pref_forward_stack.back();
        pref_forward_stack.pop_back();
        if (pref_current_row >= 0) pref_back_stack.push_back(pref_current_row);
        pref_navigating_history = true;
        GtkListBoxRow* r = gtk_list_box_get_row_at_index(GTK_LIST_BOX(preferences_sidebar), next);
        if (r) gtk_list_box_select_row(GTK_LIST_BOX(preferences_sidebar), r);
        pref_navigating_history = false;
        update_navigation_state();
        return;
    }

    GtkWidget* wv = get_webview_from_page(cur_page);
    if (wv) webkit_web_view_go_forward(WEBKIT_WEB_VIEW(wv));
}

void on_refresh(GtkWidget* w, gpointer d) {
    GtkWidget* cur_page = get_current_page_widget();
    const char* ptype = get_page_type(cur_page);
    if (strcmp(ptype, "preferences") == 0) {
        read_records(history_file, history_list);
        read_records(bookmark_file, bookmark_list);
        refresh_history_ui();
        refresh_bookmark_ui();
        refresh_cookies_from_manager();
        if (pref_dl_listbox) rebuild_download_list_ui(pref_dl_listbox, record_multi_mode);
        update_status_text(tr_full("已刷新", "已重新整理", "Refreshed"), TRUE);
        return;
    }
    GtkWidget* wv = get_webview_from_page(cur_page);
    if (!wv) return;
    webkit_web_view_reload(WEBKIT_WEB_VIEW(wv));
}

void on_new_tab(GtkWidget* w, gpointer d) { add_tab(get_home_url().c_str()); }

void on_close_tab(GtkWidget* w, gpointer d) {
    if (!notebook || !GTK_IS_NOTEBOOK(notebook)) return;
    close_tab_at_index(gtk_notebook_get_current_page(GTK_NOTEBOOK(notebook)));
}

// Firefox 风格内嵌设置子页面 (about:preferences)
void open_preferences_tab(const char* section_name) {
    if (menu_popover) gtk_widget_hide(menu_popover);
    if (!notebook || !GTK_IS_NOTEBOOK(notebook)) return;

    auto section_to_index = [](const char* sec) -> int {
        if (!sec) return 0;
        if (strcmp(sec, "search") == 0) return 1;
        if (strcmp(sec, "downloads") == 0) return 2;
        if (strcmp(sec, "privacy") == 0) return 3;
        if (strcmp(sec, "developer") == 0) return 4;
        if (strcmp(sec, "history") == 0) return 5;
        if (strcmp(sec, "bookmarks") == 0) return 6;
        if (strcmp(sec, "downloads_list") == 0) return 7;
        if (strcmp(sec, "cookies") == 0) return 8;
        if (strcmp(sec, "about") == 0) return 9;
        return 0;
    };

    if (preferences_page) {
        gint idx = gtk_notebook_page_num(GTK_NOTEBOOK(notebook), preferences_page);
        if (idx >= 0) {
            gtk_notebook_set_current_page(GTK_NOTEBOOK(notebook), idx);
            if (section_name && preferences_sidebar) {
                int row_idx = section_to_index(section_name);
                GtkListBoxRow* r = gtk_list_box_get_row_at_index(GTK_LIST_BOX(preferences_sidebar), row_idx);
                if (r) gtk_list_box_select_row(GTK_LIST_BOX(preferences_sidebar), r);
            }
            return;
        }
    }

    pref_back_stack.clear();
    pref_forward_stack.clear();
    pref_current_row = -1;
    pref_navigating_history = false;

    preferences_page = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    g_object_set_data(G_OBJECT(preferences_page), "wb_page_type", (gpointer)"preferences");

    // 左侧 Firefox 设置分类边栏（常规、搜索、文件与下载、隐私与安全、高级与开发者、关于）
    preferences_sidebar = gtk_list_box_new();
    gtk_widget_set_size_request(preferences_sidebar, 195, -1);
    gtk_style_context_add_class(gtk_widget_get_style_context(preferences_sidebar), "fx-sidebar");
    const char* cats[] = {
        tr_full("常规", "一般", "General"),
        tr_full("搜索", "搜尋", "Search"),
        tr_full("文件与下载", "檔案與下載", "Files & Downloads"),
        tr_full("隐私与安全", "隱私與安全", "Privacy & Security"),
        tr_full("高级与开发者", "進階與開發者", "Advanced & Developer"),
        tr_full("历史记录", "歷史記錄", "History"),
        tr_full("书签管理", "書籤管理", "Bookmarks"),
        tr_full("下载管理", "下載管理", "Downloads"),
        "Cookies",
        tr_full("关于 WebBrowser", "關於 WebBrowser", "About WebBrowser")
    };
    for (int i = 0; i < 10; ++i) {
        GtkWidget* r = gtk_list_box_row_new();
        GtkWidget* lbl = gtk_label_new(cats[i]);
        gtk_label_set_xalign(GTK_LABEL(lbl), 0.0f);
        gtk_container_add(GTK_CONTAINER(r), lbl);
        gtk_list_box_insert(GTK_LIST_BOX(preferences_sidebar), r, -1);
    }
    gtk_box_pack_start(GTK_BOX(preferences_page), preferences_sidebar, FALSE, FALSE, 0);

    // 右侧可滚动设置内容区
    read_records(history_file, history_list);
    read_records(bookmark_file, bookmark_list);
    record_multi_mode = false;
    record_selection_columns.clear();

    GtkWidget* right_container = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_widget_set_hexpand(right_container, TRUE);

    pref_data_toolbar = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 6);
    gtk_container_set_border_width(GTK_CONTAINER(pref_data_toolbar), 10);
    record_multi_button = gtk_button_new_with_label(tr_full("多选", "多選", "Select"));
    record_select_all_button = gtk_button_new_with_label(tr_full("全选", "全選", "Select all"));
    record_invert_button = gtk_button_new_with_label(tr_full("反选", "反選", "Invert"));
    record_delete_button = gtk_button_new_with_label(tr_full("删除选中", "刪除選中", "Delete selected"));
    record_copy_button = gtk_button_new_with_label(tr_full("复制选中", "複製選中", "Copy selected"));
    gtk_widget_set_can_focus(record_multi_button, FALSE);
    gtk_widget_set_can_focus(record_select_all_button, FALSE);
    gtk_widget_set_can_focus(record_invert_button, FALSE);
    gtk_widget_set_can_focus(record_delete_button, FALSE);
    gtk_widget_set_can_focus(record_copy_button, FALSE);
    gtk_box_pack_start(GTK_BOX(pref_data_toolbar), record_multi_button, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(pref_data_toolbar), record_select_all_button, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(pref_data_toolbar), record_invert_button, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(pref_data_toolbar), record_delete_button, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(pref_data_toolbar), record_copy_button, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(right_container), pref_data_toolbar, FALSE, FALSE, 0);

    GtkWidget* right_scroll = gtk_scrolled_window_new(NULL, NULL);
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(right_scroll), GTK_POLICY_NEVER, GTK_POLICY_AUTOMATIC);
    gtk_widget_set_hexpand(right_scroll, TRUE);
    gtk_widget_set_vexpand(right_scroll, TRUE);
    preferences_stack = gtk_stack_new();
    gtk_container_set_border_width(GTK_CONTAINER(preferences_stack), 20);
    gtk_container_add(GTK_CONTAINER(right_scroll), preferences_stack);
    gtk_box_pack_start(GTK_BOX(right_container), right_scroll, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(preferences_page), right_container, TRUE, TRUE, 0);

    g_signal_connect(record_multi_button, "clicked", G_CALLBACK(on_record_multi), NULL);
    g_signal_connect(record_select_all_button, "clicked", G_CALLBACK(on_record_select_all), NULL);
    g_signal_connect(record_invert_button, "clicked", G_CALLBACK(on_record_invert), NULL);
    g_signal_connect(record_delete_button, "clicked", G_CALLBACK(on_record_delete_selected), NULL);
    g_signal_connect(record_copy_button, "clicked", G_CALLBACK(on_record_copy_selected), NULL);

    // --- 分类 0: 常规 ---
    GtkWidget* gen_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 14);
    GtkWidget* gen_title = gtk_label_new(tr_full("常规", "一般", "General"));
    gtk_style_context_add_class(gtk_widget_get_style_context(gen_title), "fx-page-title");
    gtk_label_set_xalign(GTK_LABEL(gen_title), 0.0f);
    gtk_box_pack_start(GTK_BOX(gen_box), gen_title, FALSE, FALSE, 0);

    GtkWidget* gen_card = gtk_box_new(GTK_ORIENTATION_VERTICAL, 12);
    gtk_style_context_add_class(gtk_widget_get_style_context(gen_card), "fx-card");
    GtkWidget* gen_grid = gtk_grid_new();
    gtk_grid_set_row_spacing(GTK_GRID(gen_grid), 12);
    gtk_grid_set_column_spacing(GTK_GRID(gen_grid), 18);

    GtkWidget* theme_label = gtk_label_new(tr_full("网站外观（标准深浅色）", "網站外觀（標準深淺色）", "Website appearance"));
    gtk_widget_set_halign(theme_label, GTK_ALIGN_START);
    pref_theme_combo = gtk_combo_box_text_new();
    gtk_widget_set_halign(pref_theme_combo, GTK_ALIGN_START);
    gtk_combo_box_text_append(GTK_COMBO_BOX_TEXT(pref_theme_combo), "light", tr_full("浅色", "淺色", "Light"));
    gtk_combo_box_text_append(GTK_COMBO_BOX_TEXT(pref_theme_combo), "dark", tr_full("深色", "深色", "Dark"));
    gtk_combo_box_set_active_id(GTK_COMBO_BOX(pref_theme_combo), ui_theme.c_str());
    g_signal_connect(pref_theme_combo, "changed", G_CALLBACK(+[](GtkComboBox*, gpointer) {
        if (!pref_syncing_ui) update_pref_action_bars_visibility();
    }), NULL);
    gtk_grid_attach(GTK_GRID(gen_grid), theme_label, 0, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(gen_grid), pref_theme_combo, 1, 0, 1, 1);

    GtkWidget* lang_label = gtk_label_new(tr_full("语言", "語言", "Language"));
    gtk_widget_set_halign(lang_label, GTK_ALIGN_START);
    pref_lang_combo = gtk_combo_box_text_new();
    gtk_widget_set_halign(pref_lang_combo, GTK_ALIGN_START);
    gtk_combo_box_text_append(GTK_COMBO_BOX_TEXT(pref_lang_combo), "zh", "简体中文");
    gtk_combo_box_text_append(GTK_COMBO_BOX_TEXT(pref_lang_combo), "zh-TW", "繁體中文");
    gtk_combo_box_text_append(GTK_COMBO_BOX_TEXT(pref_lang_combo), "en", "English");
    gtk_combo_box_set_active_id(GTK_COMBO_BOX(pref_lang_combo), ui_language.c_str());
    g_signal_connect(pref_lang_combo, "changed", G_CALLBACK(+[](GtkComboBox*, gpointer) {
        if (!pref_syncing_ui) update_pref_action_bars_visibility();
    }), NULL);
    gtk_grid_attach(GTK_GRID(gen_grid), lang_label, 0, 1, 1, 1);
    gtk_grid_attach(GTK_GRID(gen_grid), pref_lang_combo, 1, 1, 1, 1);

    GtkWidget* zoom_label = gtk_label_new(tr_full("默认缩放", "預設縮放", "Default Zoom"));
    gtk_widget_set_halign(zoom_label, GTK_ALIGN_START);
    pref_zoom_scale = gtk_scale_new_with_range(GTK_ORIENTATION_HORIZONTAL, 0.0, 100.0, 1.0);
    gtk_widget_set_size_request(pref_zoom_scale, 240, -1);
    gtk_widget_set_halign(pref_zoom_scale, GTK_ALIGN_START);
    gtk_range_set_value(GTK_RANGE(pref_zoom_scale), page_zoom_percent);
    gtk_scale_set_digits(GTK_SCALE(pref_zoom_scale), 0);
    gtk_scale_set_value_pos(GTK_SCALE(pref_zoom_scale), GTK_POS_RIGHT);
    gtk_scale_add_mark(GTK_SCALE(pref_zoom_scale), 0.0, GTK_POS_BOTTOM, "0%");
    gtk_scale_add_mark(GTK_SCALE(pref_zoom_scale), 50.0, GTK_POS_BOTTOM, "50%");
    gtk_scale_add_mark(GTK_SCALE(pref_zoom_scale), 100.0, GTK_POS_BOTTOM, "100%");
    g_signal_connect(pref_zoom_scale, "value-changed", G_CALLBACK(+[](GtkRange*, gpointer) {
        if (!pref_syncing_ui) update_pref_action_bars_visibility();
    }), NULL);
    gtk_grid_attach(GTK_GRID(gen_grid), zoom_label, 0, 2, 1, 1);
    gtk_grid_attach(GTK_GRID(gen_grid), pref_zoom_scale, 1, 2, 1, 1);

    gtk_box_pack_start(GTK_BOX(gen_card), gen_grid, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(gen_box), gen_card, FALSE, FALSE, 0);

    pref_gen_action_bar = create_pref_action_bar(G_CALLBACK(+[](GtkWidget*, gpointer) {
        bool theme_changed = false;
        bool lang_changed = false;
        bool zoom_changed = false;
        if (pref_theme_combo) {
            const gchar* id = gtk_combo_box_get_active_id(GTK_COMBO_BOX(pref_theme_combo));
            if (id && ui_theme != id) { ui_theme = id; theme_changed = true; }
        }
        if (pref_lang_combo) {
            const gchar* id = gtk_combo_box_get_active_id(GTK_COMBO_BOX(pref_lang_combo));
            if (id && ui_language != id) { ui_language = id; lang_changed = true; }
        }
        if (pref_zoom_scale) {
            double val = gtk_range_get_value(GTK_RANGE(pref_zoom_scale));
            if (std::lround(val) != std::lround(page_zoom_percent)) {
                page_zoom_percent = val;
                zoom_changed = true;
            }
        }
        save_settings();
        if (theme_changed) apply_app_theme();
        if (lang_changed) {
            apply_ui_language();
            apply_web_settings_to_all_tabs(true);
        }
        if (zoom_changed) apply_zoom_to_all_tabs();
        update_pref_action_bars_visibility();
        update_status_text(tr_full("常规设置已应用", "一般設定已套用", "General settings applied"), TRUE);
    }));
    gtk_box_pack_start(GTK_BOX(gen_box), pref_gen_action_bar, FALSE, FALSE, 0);
    gtk_stack_add_named(GTK_STACK(preferences_stack), gen_box, "general");

    // --- 分类 1: 搜索 ---
    GtkWidget* srch_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 14);
    GtkWidget* srch_title = gtk_label_new(tr_full("搜索", "搜尋", "Search"));
    gtk_style_context_add_class(gtk_widget_get_style_context(srch_title), "fx-page-title");
    gtk_label_set_xalign(GTK_LABEL(srch_title), 0.0f);
    gtk_box_pack_start(GTK_BOX(srch_box), srch_title, FALSE, FALSE, 0);

    GtkWidget* srch_card = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_style_context_add_class(gtk_widget_get_style_context(srch_card), "fx-card");
    GtkWidget* srch_row = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 16);
    GtkWidget* search_label = gtk_label_new(tr_full("默认搜索引擎", "預設搜尋引擎", "Default Search Engine"));
    gtk_widget_set_halign(search_label, GTK_ALIGN_START);
    pref_search_combo = gtk_combo_box_text_new();
    gtk_combo_box_text_append(GTK_COMBO_BOX_TEXT(pref_search_combo), "google", tr_full("谷歌 (Google)", "Google", "Google"));
    gtk_combo_box_text_append(GTK_COMBO_BOX_TEXT(pref_search_combo), "bing", tr_full("必应 (Bing)", "Bing", "Bing"));
    gtk_combo_box_text_append(GTK_COMBO_BOX_TEXT(pref_search_combo), "baidu", tr_full("百度 (Baidu)", "百度 (Baidu)", "Baidu"));
    gtk_combo_box_set_active_id(GTK_COMBO_BOX(pref_search_combo), search_engine.c_str());
    g_signal_connect(pref_search_combo, "changed", G_CALLBACK(+[](GtkComboBox*, gpointer) {
        if (!pref_syncing_ui) update_pref_action_bars_visibility();
    }), NULL);
    gtk_box_pack_start(GTK_BOX(srch_row), search_label, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(srch_row), pref_search_combo, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(srch_card), srch_row, FALSE, FALSE, 0);
    GtkWidget* srch_desc = gtk_label_new(tr_full("地址栏搜索、新标签页与划词搜索将使用所选搜索引擎。", "網址列搜尋、新分頁與劃詞搜尋將使用所選搜尋引擎。", "Used for address bar searches, new tabs, and text selection search."));
    gtk_label_set_xalign(GTK_LABEL(srch_desc), 0.0f);
    gtk_label_set_line_wrap(GTK_LABEL(srch_desc), TRUE);
    gtk_box_pack_start(GTK_BOX(srch_card), srch_desc, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(srch_box), srch_card, FALSE, FALSE, 0);

    pref_srch_action_bar = create_pref_action_bar(G_CALLBACK(+[](GtkWidget*, gpointer) {
        if (pref_search_combo) {
            const gchar* id = gtk_combo_box_get_active_id(GTK_COMBO_BOX(pref_search_combo));
            if (id) {
                search_engine = id;
                save_settings();
                update_search_engine_in_all_tabs();
            }
        }
        update_pref_action_bars_visibility();
        update_status_text(tr_full("搜索设置已应用", "搜尋設定已套用", "Search settings applied"), TRUE);
    }));
    gtk_box_pack_start(GTK_BOX(srch_box), pref_srch_action_bar, FALSE, FALSE, 0);
    gtk_stack_add_named(GTK_STACK(preferences_stack), srch_box, "search");

    // --- 分类 2: 文件与下载 ---
    GtkWidget* dl_box_page = gtk_box_new(GTK_ORIENTATION_VERTICAL, 14);
    GtkWidget* dl_title = gtk_label_new(tr_full("文件与下载", "檔案與下載", "Files and Downloads"));
    gtk_style_context_add_class(gtk_widget_get_style_context(dl_title), "fx-page-title");
    gtk_label_set_xalign(GTK_LABEL(dl_title), 0.0f);
    gtk_box_pack_start(GTK_BOX(dl_box_page), dl_title, FALSE, FALSE, 0);

    GtkWidget* dl_card = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_style_context_add_class(gtk_widget_get_style_context(dl_card), "fx-card");
    GtkWidget* dl_row = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 8);
    GtkWidget* dl_lbl = gtk_label_new(tr_full("保存文件至", "儲存檔案至", "Save files to"));
    pref_dl_entry = gtk_entry_new();
    gtk_entry_set_text(GTK_ENTRY(pref_dl_entry), download_dir.c_str());
    gtk_widget_set_hexpand(pref_dl_entry, TRUE);
    GtkWidget* dl_browse = gtk_button_new_with_label(tr_full("浏览...", "瀏覽...", "Browse..."));
    g_signal_connect(pref_dl_entry, "changed", G_CALLBACK(+[](GtkEditable*, gpointer) {
        if (!pref_syncing_ui) update_pref_action_bars_visibility();
    }), NULL);
    g_signal_connect(dl_browse, "clicked", G_CALLBACK(+[](GtkWidget*, gpointer data) {
        GtkEntry* entry = GTK_ENTRY(data);
        GtkWidget* chooser = gtk_file_chooser_dialog_new(
            tr_full("选择下载文件夹", "選擇下載資料夾", "Select download folder"),
            GTK_WINDOW(main_window),
            GTK_FILE_CHOOSER_ACTION_SELECT_FOLDER,
            tr_full("取消", "取消", "Cancel"), GTK_RESPONSE_CANCEL,
            tr_full("选择", "選擇", "Select"), GTK_RESPONSE_ACCEPT,
            NULL);
        const gchar* cur = gtk_entry_get_text(entry);
        if (cur && *cur) gtk_file_chooser_set_current_folder(GTK_FILE_CHOOSER(chooser), cur);
        if (gtk_dialog_run(GTK_DIALOG(chooser)) == GTK_RESPONSE_ACCEPT) {
            gchar* folder = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(chooser));
            if (folder) {
                gtk_entry_set_text(entry, folder);
                g_free(folder);
            }
        }
        gtk_widget_destroy(chooser);
    }), pref_dl_entry);
    gtk_box_pack_start(GTK_BOX(dl_row), dl_lbl, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(dl_row), pref_dl_entry, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(dl_row), dl_browse, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(dl_card), dl_row, FALSE, FALSE, 0);

    GtkWidget* open_dl_mgr = gtk_button_new_with_label(tr_full("打开下载管理", "開啟下載管理", "Open Downloads Manager"));
    gtk_widget_set_halign(open_dl_mgr, GTK_ALIGN_START);
    g_signal_connect(open_dl_mgr, "clicked", G_CALLBACK(+[](GtkWidget*, gpointer) {
        open_preferences_tab("downloads_list");
    }), NULL);
    gtk_box_pack_start(GTK_BOX(dl_card), open_dl_mgr, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(dl_box_page), dl_card, FALSE, FALSE, 0);

    pref_dl_action_bar = create_pref_action_bar(G_CALLBACK(+[](GtkWidget*, gpointer) {
        if (pref_dl_entry) {
            std::string val = trim_whitespace(gtk_entry_get_text(GTK_ENTRY(pref_dl_entry)));
            if (!val.empty()) {
                download_dir = val;
                ensure_dir_recursive(download_dir);
                save_settings();
            } else {
                reset_all_pref_drafts();
                return;
            }
        }
        update_pref_action_bars_visibility();
        update_status_text(tr_full("下载设置已应用", "下載設定已套用", "Download settings applied"), TRUE);
    }));
    gtk_box_pack_start(GTK_BOX(dl_box_page), pref_dl_action_bar, FALSE, FALSE, 0);
    gtk_stack_add_named(GTK_STACK(preferences_stack), dl_box_page, "downloads");

    // --- 分类 3: 隐私与安全（真正的 Cookie 策略、缓存清理、HTTPS 证书、自动播放权限、广告拦截） ---
    GtkWidget* priv_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 14);
    GtkWidget* priv_title = gtk_label_new(tr_full("隐私与安全", "隱私與安全", "Privacy & Security"));
    gtk_style_context_add_class(gtk_widget_get_style_context(priv_title), "fx-page-title");
    gtk_label_set_xalign(GTK_LABEL(priv_title), 0.0f);
    gtk_box_pack_start(GTK_BOX(priv_box), priv_title, FALSE, FALSE, 0);

    // 卡片 A: Cookie 与网站数据
    GtkWidget* cookie_card = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_style_context_add_class(gtk_widget_get_style_context(cookie_card), "fx-card");
    GtkWidget* cookie_hdr = gtk_label_new(tr_full("Cookies 与网站数据", "Cookies 與網站資料", "Cookies and Site Data"));
    gtk_style_context_add_class(gtk_widget_get_style_context(cookie_hdr), "fx-section-title");
    gtk_label_set_xalign(GTK_LABEL(cookie_hdr), 0.0f);
    gtk_box_pack_start(GTK_BOX(cookie_card), cookie_hdr, FALSE, FALSE, 0);

    GtkWidget* cookie_row = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 12);
    GtkWidget* cookie_lbl = gtk_label_new(tr_full("Cookies 接收策略", "Cookies 接收原則", "Cookies policy"));
    pref_cookie_combo = gtk_combo_box_text_new();
    gtk_combo_box_text_append(GTK_COMBO_BOX_TEXT(pref_cookie_combo), "always", tr_full("允许所有 Cookies", "允許所有 Cookies", "Allow all Cookies"));
    gtk_combo_box_text_append(GTK_COMBO_BOX_TEXT(pref_cookie_combo), "no_third_party", tr_full("仅允许第一方（阻止第三方追踪 Cookies）", "僅允許第一方（封鎖第三方追蹤 Cookies）", "Block third-party Cookies"));
    gtk_combo_box_text_append(GTK_COMBO_BOX_TEXT(pref_cookie_combo), "never", tr_full("阻止所有 Cookies", "封鎖所有 Cookies", "Block all Cookies"));
    gtk_combo_box_set_active_id(GTK_COMBO_BOX(pref_cookie_combo), cookie_policy.c_str());
    g_signal_connect(pref_cookie_combo, "changed", G_CALLBACK(+[](GtkComboBox*, gpointer) {
        if (!pref_syncing_ui) update_pref_action_bars_visibility();
    }), NULL);
    gtk_box_pack_start(GTK_BOX(cookie_row), cookie_lbl, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(cookie_row), pref_cookie_combo, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(cookie_card), cookie_row, FALSE, FALSE, 0);

    GtkWidget* data_btns = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 8);
    GtkWidget* open_cookies_btn = gtk_button_new_with_label(tr_full("查看 Cookies...", "檢視 Cookies...", "View Cookies..."));
    GtkWidget* clear_cache_btn = gtk_button_new_with_label(tr_full("清除网页缓存", "清除網頁快取", "Clear web cache"));
    g_signal_connect(open_cookies_btn, "clicked", G_CALLBACK(+[](GtkWidget*, gpointer) {
        open_preferences_tab("cookies");
    }), NULL);
    g_signal_connect(clear_cache_btn, "clicked", G_CALLBACK(+[](GtkWidget*, gpointer) {
        if (data_manager) {
            webkit_website_data_manager_clear(
                data_manager,
                (WebKitWebsiteDataTypes)(WEBKIT_WEBSITE_DATA_DISK_CACHE | WEBKIT_WEBSITE_DATA_MEMORY_CACHE),
                0, NULL, NULL, NULL);
            update_status_text(tr_full("已清除网页缓存", "已清除網頁快取", "Web cache cleared"), TRUE);
        }
    }), NULL);
    gtk_box_pack_start(GTK_BOX(data_btns), open_cookies_btn, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(data_btns), clear_cache_btn, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(cookie_card), data_btns, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(priv_box), cookie_card, FALSE, FALSE, 0);

    // 卡片 B: 权限与连接安全
    GtkWidget* sec_card = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_style_context_add_class(gtk_widget_get_style_context(sec_card), "fx-card");
    GtkWidget* sec_hdr = gtk_label_new(tr_full("权限与连接安全", "權限與連線安全", "Permissions & Security"));
    gtk_style_context_add_class(gtk_widget_get_style_context(sec_hdr), "fx-section-title");
    gtk_label_set_xalign(GTK_LABEL(sec_hdr), 0.0f);
    gtk_box_pack_start(GTK_BOX(sec_card), sec_hdr, FALSE, FALSE, 0);

    pref_media_check = gtk_check_button_new_with_label(tr_full("阻止网站自动播放音视频（需要用户点击）", "封鎖網站自動播放影音（需要使用者點擊）", "Block autoplay of audio and video"));
    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(pref_media_check), media_requires_gesture);
    g_signal_connect(pref_media_check, "toggled", G_CALLBACK(+[](GtkToggleButton*, gpointer) {
        if (!pref_syncing_ui) update_pref_action_bars_visibility();
    }), NULL);

    pref_tls_check = gtk_check_button_new_with_label(tr_full("忽略无效或自签名 HTTPS 证书警告", "忽略無效或自簽名 HTTPS 憑證警告", "Ignore invalid or self-signed HTTPS certificate errors"));
    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(pref_tls_check), ignore_tls_errors);
    g_signal_connect(pref_tls_check, "toggled", G_CALLBACK(+[](GtkToggleButton*, gpointer) {
        if (!pref_syncing_ui) update_pref_action_bars_visibility();
    }), NULL);

    gtk_box_pack_start(GTK_BOX(sec_card), pref_media_check, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(sec_card), pref_tls_check, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(priv_box), sec_card, FALSE, FALSE, 0);

    // 卡片 C: 自定义广告拦截（默认无规则，需用户自行写入）
    GtkWidget* ad_card = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_style_context_add_class(gtk_widget_get_style_context(ad_card), "fx-card");
    GtkWidget* ad_hdr = gtk_label_new(tr_full("广告拦截", "廣告攔截", "Ad Blocker"));
    gtk_style_context_add_class(gtk_widget_get_style_context(ad_hdr), "fx-section-title");
    gtk_label_set_xalign(GTK_LABEL(ad_hdr), 0.0f);
    gtk_box_pack_start(GTK_BOX(ad_card), ad_hdr, FALSE, FALSE, 0);

    pref_ad_check = gtk_check_button_new_with_label(tr_full("启用自定义广告拦截", "啟用自訂廣告攔截", "Enable custom ad blocker"));
    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(pref_ad_check), adblock_enabled);
    g_signal_connect(pref_ad_check, "toggled", G_CALLBACK(+[](GtkToggleButton*, gpointer) {
        if (!pref_syncing_ui) update_pref_action_bars_visibility();
    }), NULL);
    gtk_box_pack_start(GTK_BOX(ad_card), pref_ad_check, FALSE, FALSE, 0);

    GtkWidget* ad_hint = gtk_label_new(tr_full("每行填写一条拦截域名或关键词（默认无内置规则，# 开头为注释，例如：doubleclick.net）：", "每行填寫一條攔截網域或關鍵字（預設無內建規則，# 開頭為註解，例如：doubleclick.net）：", "One domain or keyword per line (empty by default, # for comments, e.g. doubleclick.net):"));
    gtk_label_set_xalign(GTK_LABEL(ad_hint), 0.0f);
    gtk_label_set_line_wrap(GTK_LABEL(ad_hint), TRUE);
    gtk_box_pack_start(GTK_BOX(ad_card), ad_hint, FALSE, FALSE, 0);

    GtkWidget* ad_scroll = gtk_scrolled_window_new(NULL, NULL);
    gtk_widget_set_size_request(ad_scroll, -1, 130);
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(ad_scroll), GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
    GtkWidget* ad_textview = gtk_text_view_new();
    gtk_text_view_set_wrap_mode(GTK_TEXT_VIEW(ad_textview), GTK_WRAP_WORD_CHAR);
    pref_ad_buf = gtk_text_view_get_buffer(GTK_TEXT_VIEW(ad_textview));
    gtk_text_buffer_set_text(pref_ad_buf, adblock_rules_text.c_str(), -1);
    g_signal_connect(pref_ad_buf, "changed", G_CALLBACK(+[](GtkTextBuffer*, gpointer) {
        if (!pref_syncing_ui) update_pref_action_bars_visibility();
    }), NULL);
    gtk_container_add(GTK_CONTAINER(ad_scroll), ad_textview);
    gtk_box_pack_start(GTK_BOX(ad_card), ad_scroll, FALSE, FALSE, 0);

    adblock_status_lbl = gtk_label_new("");
    gtk_label_set_xalign(GTK_LABEL(adblock_status_lbl), 0.0f);
    gtk_box_pack_start(GTK_BOX(ad_card), adblock_status_lbl, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(priv_box), ad_card, FALSE, FALSE, 0);
    apply_adblock_rules();

    pref_priv_action_bar = create_pref_action_bar(G_CALLBACK(+[](GtkWidget*, gpointer) {
        bool policy_changed = false;
        bool media_changed = false;
        bool ad_changed = false;
        if (pref_cookie_combo) {
            const gchar* id = gtk_combo_box_get_active_id(GTK_COMBO_BOX(pref_cookie_combo));
            if (id && cookie_policy != id) { cookie_policy = id; policy_changed = true; }
        }
        if (pref_media_check) {
            bool val = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(pref_media_check));
            if (val != media_requires_gesture) { media_requires_gesture = val; media_changed = true; }
        }
        if (pref_tls_check) {
            bool val = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(pref_tls_check));
            if (val != ignore_tls_errors) { ignore_tls_errors = val; policy_changed = true; }
        }
        if (pref_ad_check) {
            bool val = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(pref_ad_check));
            if (val != adblock_enabled) { adblock_enabled = val; ad_changed = true; }
        }
        if (pref_ad_buf) {
            std::string rules = get_pref_ad_buffer_text();
            if (rules != adblock_rules_text) {
                save_adblock_rules(rules);
                ad_changed = true;
            }
        }
        save_settings();
        if (policy_changed) apply_privacy_and_security_policies();
        if (media_changed) apply_web_settings_to_all_tabs(false);
        if (ad_changed) apply_adblock_rules();
        update_pref_action_bars_visibility();
        update_status_text(tr_full("隐私与安全设置已应用", "隱私與安全設定已套用", "Privacy & security settings applied"), TRUE);
    }));
    gtk_box_pack_start(GTK_BOX(priv_box), pref_priv_action_bar, FALSE, FALSE, 0);
    gtk_stack_add_named(GTK_STACK(preferences_stack), priv_box, "privacy");

    // --- 分类 4: 高级与开发者 ---
    GtkWidget* dev_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 14);
    GtkWidget* dev_title = gtk_label_new(tr_full("高级与开发者", "進階與開發者", "Advanced & Developer"));
    gtk_style_context_add_class(gtk_widget_get_style_context(dev_title), "fx-page-title");
    gtk_label_set_xalign(GTK_LABEL(dev_title), 0.0f);
    gtk_box_pack_start(GTK_BOX(dev_box), dev_title, FALSE, FALSE, 0);

    GtkWidget* dev_card = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_style_context_add_class(gtk_widget_get_style_context(dev_card), "fx-card");
    pref_js_check = gtk_check_button_new_with_label(tr_full("启用 JavaScript 脚本引擎", "啟用 JavaScript 指令碼引擎", "Enable JavaScript"));
    pref_webgl_check = gtk_check_button_new_with_label(tr_full("启用 WebGL 硬件图形渲染", "啟用 WebGL 硬體圖形算繪", "Enable WebGL"));
    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(pref_js_check), javascript_enabled);
    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(pref_webgl_check), webgl_enabled);
    g_signal_connect(pref_js_check, "toggled", G_CALLBACK(+[](GtkToggleButton*, gpointer) {
        if (!pref_syncing_ui) update_pref_action_bars_visibility();
    }), NULL);
    g_signal_connect(pref_webgl_check, "toggled", G_CALLBACK(+[](GtkToggleButton*, gpointer) {
        if (!pref_syncing_ui) update_pref_action_bars_visibility();
    }), NULL);
    gtk_box_pack_start(GTK_BOX(dev_card), pref_js_check, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(dev_card), pref_webgl_check, FALSE, FALSE, 0);

    gtk_box_pack_start(GTK_BOX(dev_box), dev_card, FALSE, FALSE, 0);

    pref_dev_action_bar = create_pref_action_bar(G_CALLBACK(+[](GtkWidget*, gpointer) {
        if (pref_js_check) javascript_enabled = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(pref_js_check));
        if (pref_webgl_check) webgl_enabled = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(pref_webgl_check));
        save_settings();
        apply_web_settings_to_all_tabs(false);
        update_pref_action_bars_visibility();
        update_status_text(tr_full("高级设置已应用", "進階設定已套用", "Advanced settings applied"), TRUE);
    }));
    gtk_box_pack_start(GTK_BOX(dev_box), pref_dev_action_bar, FALSE, FALSE, 0);
    gtk_stack_add_named(GTK_STACK(preferences_stack), dev_box, "developer");

    // --- 分类 5: 历史记录 ---
    GtkWidget* scroll_hist = gtk_scrolled_window_new(NULL, NULL);
    gtk_widget_set_vexpand(scroll_hist, TRUE);
    history_store = gtk_list_store_new(5, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_BOOLEAN, G_TYPE_INT);
    GtkWidget* tree_hist = create_record_tree(history_store, false);
    g_object_unref(history_store);
    gtk_container_add(GTK_CONTAINER(scroll_hist), tree_hist);
    gtk_stack_add_named(GTK_STACK(preferences_stack), scroll_hist, "history");
    refresh_history_ui();

    // --- 分类 6: 书签 ---
    GtkWidget* scroll_bm = gtk_scrolled_window_new(NULL, NULL);
    gtk_widget_set_vexpand(scroll_bm, TRUE);
    bookmark_store = gtk_list_store_new(5, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_BOOLEAN, G_TYPE_INT);
    GtkWidget* tree_bm = create_record_tree(bookmark_store, false);
    g_object_unref(bookmark_store);
    gtk_container_add(GTK_CONTAINER(scroll_bm), tree_bm);
    gtk_stack_add_named(GTK_STACK(preferences_stack), scroll_bm, "bookmarks");
    refresh_bookmark_ui();

    // --- 分类 7: 下载管理 ---
    GtkWidget* scroll_dl_tab = gtk_scrolled_window_new(NULL, NULL);
    gtk_widget_set_vexpand(scroll_dl_tab, TRUE);
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scroll_dl_tab), GTK_POLICY_NEVER, GTK_POLICY_AUTOMATIC);
    pref_dl_listbox = gtk_list_box_new();
    gtk_list_box_set_selection_mode(GTK_LIST_BOX(pref_dl_listbox), GTK_SELECTION_NONE);
    gtk_container_add(GTK_CONTAINER(scroll_dl_tab), pref_dl_listbox);
    gtk_stack_add_named(GTK_STACK(preferences_stack), scroll_dl_tab, "downloads_list");

    // --- 分类 8: Cookies ---
    GtkWidget* scroll_ck = gtk_scrolled_window_new(NULL, NULL);
    gtk_widget_set_vexpand(scroll_ck, TRUE);
    cookie_store = gtk_list_store_new(4, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_BOOLEAN, G_TYPE_INT);
    GtkWidget* tree_ck = gtk_tree_view_new_with_model(GTK_TREE_MODEL(cookie_store));
    g_object_unref(cookie_store);
    GtkCellRenderer* cookie_toggle = gtk_cell_renderer_toggle_new();
    GtkTreeViewColumn* cookie_select_col = gtk_tree_view_column_new_with_attributes(tr_full("选择", "選擇", "Select"), cookie_toggle, "active", 2, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(tree_ck), cookie_select_col);
    record_selection_columns.push_back(cookie_select_col);
    g_signal_connect(cookie_toggle, "toggled", G_CALLBACK(on_record_toggle), cookie_store);
    GtkCellRenderer* cr = gtk_cell_renderer_text_new();
    g_object_set(cr, "ellipsize", PANGO_ELLIPSIZE_END, "ellipsize-set", TRUE, "width-chars", 22, "max-width-chars", 36, NULL);
    GtkTreeViewColumn* cookie_domain_col = gtk_tree_view_column_new_with_attributes(tr_full("域名", "網域", "Domain"), cr, "text", 0, NULL);
    gtk_tree_view_column_set_expand(cookie_domain_col, TRUE);
    gtk_tree_view_append_column(GTK_TREE_VIEW(tree_ck), cookie_domain_col);
    cr = gtk_cell_renderer_text_new();
    g_object_set(cr, "ellipsize", PANGO_ELLIPSIZE_END, "ellipsize-set", TRUE, "width-chars", 18, "max-width-chars", 30, NULL);
    GtkTreeViewColumn* cookie_name_col = gtk_tree_view_column_new_with_attributes(tr_full("名称", "名稱", "Name"), cr, "text", 1, NULL);
    gtk_tree_view_column_set_expand(cookie_name_col, TRUE);
    gtk_tree_view_append_column(GTK_TREE_VIEW(tree_ck), cookie_name_col);
    gtk_container_add(GTK_CONTAINER(scroll_ck), tree_ck);
    gtk_stack_add_named(GTK_STACK(preferences_stack), scroll_ck, "cookies");
    refresh_cookies_from_manager();

    // --- 分类 9: 关于 ---
    GtkWidget* about_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 14);
    GtkWidget* about_title = gtk_label_new(tr_full("关于 WebBrowser", "關於 WebBrowser", "About WebBrowser"));
    gtk_style_context_add_class(gtk_widget_get_style_context(about_title), "fx-page-title");
    gtk_label_set_xalign(GTK_LABEL(about_title), 0.0f);
    gtk_box_pack_start(GTK_BOX(about_box), about_title, FALSE, FALSE, 0);

    GtkWidget* about_card = gtk_box_new(GTK_ORIENTATION_VERTICAL, 12);
    gtk_style_context_add_class(gtk_widget_get_style_context(about_card), "fx-card");
    GtkWidget* about_header = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 14);
    GdkPixbuf* logo_pb = gdk_pixbuf_new_from_file_at_scale(app_icon_path.c_str(), 64, 64, TRUE, NULL);
    if (logo_pb) {
        GtkWidget* logo_img = gtk_image_new_from_pixbuf(logo_pb);
        g_object_unref(logo_pb);
        gtk_box_pack_start(GTK_BOX(about_header), logo_img, FALSE, FALSE, 0);
    } else {
        gtk_box_pack_start(GTK_BOX(about_header), create_vector_icon(ICON_APP_LOGO, 48), FALSE, FALSE, 0);
    }
    GtkWidget* about_name = gtk_label_new("WebBrowser");
    gtk_style_context_add_class(gtk_widget_get_style_context(about_name), "fx-page-title");
    gtk_label_set_xalign(GTK_LABEL(about_name), 0.0f);
    gtk_box_pack_start(GTK_BOX(about_header), about_name, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(about_card), about_header, FALSE, FALSE, 0);
    GtkWidget* about_lbl = gtk_label_new(NULL);
    gtk_label_set_xalign(GTK_LABEL(about_lbl), 0.0f);
    gtk_label_set_line_wrap(GTK_LABEL(about_lbl), TRUE);
    gtk_label_set_selectable(GTK_LABEL(about_lbl), TRUE);
    const char* markup =
        "<b>WebBrowser</b>\n\n"
        "由 Sakuragi Group 开发的轻量级 Web 浏览器，面向 Termux:X11 环境。\n"
        "A lightweight web browser developed by Sakuragi Group for the Termux:X11 environment.\n\n"
        "制作团队 / Developer: Sakuragi Group\n"
        "Copyright © Sakuragi Group\n"
        "项目仓库 / Repository: <a href='https://github.com/YamaiSakuragi/WebBrowser'>https://github.com/YamaiSakuragi/WebBrowser</a>\n"
        "Telegram: <a href='https://t.me/YamaiSakuragi/'>https://t.me/YamaiSakuragi/</a>";
    gtk_label_set_markup(GTK_LABEL(about_lbl), markup);
    g_signal_connect(about_lbl, "activate-link", G_CALLBACK(+[](GtkLabel*, const gchar* uri, gpointer) -> gboolean {
        if (uri && *uri) add_tab(uri);
        return TRUE;
    }), NULL);
    gtk_box_pack_start(GTK_BOX(about_card), about_lbl, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(about_box), about_card, FALSE, FALSE, 0);
    gtk_stack_add_named(GTK_STACK(preferences_stack), about_box, "about");

    g_signal_connect(preferences_sidebar, "row-selected", G_CALLBACK(+[](GtkListBox*, GtkListBoxRow* row, gpointer) {
        if (!row || !preferences_stack) return;
        reset_all_pref_drafts();
        int idx = gtk_list_box_row_get_index(row);
        if (!pref_navigating_history && pref_current_row >= 0 && pref_current_row != idx) {
            pref_back_stack.push_back(pref_current_row);
            pref_forward_stack.clear();
        }
        pref_current_row = idx;
        const char* name = nullptr;
        if (idx == 0) name = "general";
        else if (idx == 1) name = "search";
        else if (idx == 2) name = "downloads";
        else if (idx == 3) name = "privacy";
        else if (idx == 4) name = "developer";
        else if (idx == 5) name = "history";
        else if (idx == 6) name = "bookmarks";
        else if (idx == 7) name = "downloads_list";
        else if (idx == 8) name = "cookies";
        else if (idx == 9) name = "about";
        if (name) {
            gtk_stack_set_visible_child_name(GTK_STACK(preferences_stack), name);
        }
        const bool is_data = (idx >= 5 && idx <= 8);
        if (pref_data_toolbar) gtk_widget_set_visible(pref_data_toolbar, is_data);
        set_record_multi_mode(false);
        if (idx == 7 && pref_dl_listbox) {
            rebuild_download_list_ui(pref_dl_listbox, false);
        }
        if (url_entry && GTK_IS_ENTRY(url_entry) && get_current_page_widget() == preferences_page) {
            gtk_entry_set_text(GTK_ENTRY(url_entry), get_current_preferences_url());
        }
        sync_tab_bar();
        update_navigation_state();
    }), NULL);

    g_signal_connect(preferences_page, "destroy", G_CALLBACK(+[](GtkWidget*, gpointer) {
        if (cookie_fetch_cancellable) {
            g_cancellable_cancel(cookie_fetch_cancellable);
            g_object_unref(cookie_fetch_cancellable);
            cookie_fetch_cancellable = nullptr;
        }
        if (active_download_listbox == pref_dl_listbox) {
            clear_download_row_widgets();
            active_download_listbox = nullptr;
            active_download_multi_mode = false;
        }
        for (const auto& item : download_list) item->selected = false;
        preferences_page = nullptr;
        preferences_sidebar = nullptr;
        preferences_stack = nullptr;
        pref_data_toolbar = nullptr;
        pref_theme_combo = nullptr;
        pref_lang_combo = nullptr;
        pref_zoom_scale = nullptr;
        pref_gen_action_bar = nullptr;
        pref_search_combo = nullptr;
        pref_srch_action_bar = nullptr;
        pref_dl_entry = nullptr;
        pref_dl_action_bar = nullptr;
        pref_cookie_combo = nullptr;
        pref_media_check = nullptr;
        pref_tls_check = nullptr;
        pref_ad_check = nullptr;
        pref_ad_buf = nullptr;
        pref_priv_action_bar = nullptr;
        pref_js_check = nullptr;
        pref_webgl_check = nullptr;
        pref_dev_action_bar = nullptr;
        pref_dl_listbox = nullptr;
        adblock_status_lbl = nullptr;
        history_store = bookmark_store = cookie_store = nullptr;
        record_selection_columns.clear();
        record_multi_button = record_select_all_button = record_invert_button = record_delete_button = record_copy_button = nullptr;
        pref_back_stack.clear();
        pref_forward_stack.clear();
        pref_current_row = -1;
    }), NULL);

    gtk_widget_show_all(preferences_page);
    set_record_multi_mode(false);
    int page = gtk_notebook_append_page(GTK_NOTEBOOK(notebook), preferences_page, NULL);
    gtk_notebook_set_current_page(GTK_NOTEBOOK(notebook), page);

    int initial_row_idx = section_to_index(section_name);
    GtkListBoxRow* initial_row = gtk_list_box_get_row_at_index(GTK_LIST_BOX(preferences_sidebar), initial_row_idx);
    if (initial_row) gtk_list_box_select_row(GTK_LIST_BOX(preferences_sidebar), initial_row);

    sync_tab_bar();
    update_navigation_state();
}

void on_main_window_destroy(GtkWidget*, gpointer) {
    if (status_hide_timer > 0) {
        g_source_remove(status_hide_timer);
        status_hide_timer = 0;
    }
    if (resize_timer_id > 0) {
        g_source_remove(resize_timer_id);
        resize_timer_id = 0;
    }
    if (cookie_fetch_cancellable) {
        g_cancellable_cancel(cookie_fetch_cancellable);
        g_object_unref(cookie_fetch_cancellable);
        cookie_fetch_cancellable = nullptr;
    }
    for (const auto& item : download_list) {
        if (item->wk_download) {
            webkit_download_cancel(item->wk_download);
            g_object_unref(item->wk_download);
            item->wk_download = nullptr;
        }
    }
    save_downloads();
    gtk_main_quit();
}

GtkWidget* create_menu_item_button(const char* label) {
    GtkWidget* btn = gtk_button_new_with_label(label);
    gtk_widget_set_can_focus(btn, FALSE);
    gtk_button_set_relief(GTK_BUTTON(btn), GTK_RELIEF_NONE);
    gtk_style_context_add_class(gtk_widget_get_style_context(btn), "menu-item-btn");
    GtkWidget* child = gtk_bin_get_child(GTK_BIN(btn));
    if (child && GTK_IS_LABEL(child)) gtk_label_set_xalign(GTK_LABEL(child), 0.0f);
    return btn;
}

void build_ui(GtkWidget* window) {
    mkdir("/data/data/com.termux/files/home/.config", 0777);
    mkdir("/data/data/com.termux/files/home/.config/WebBrowser", 0777);
    mkdir(base_dir.c_str(), 0777);
    mkdir((base_dir + "/history").c_str(), 0777);
    mkdir((base_dir + "/bookmarks").c_str(), 0777);
    mkdir((base_dir + "/downloads").c_str(), 0777);
    mkdir((base_dir + "/adblock").c_str(), 0777);
    mkdir((base_dir + "/cookies").c_str(), 0777);
    mkdir((base_dir + "/cache").c_str(), 0777);

    load_settings();
    ensure_dir_recursive(download_dir);
    apply_app_theme();
    read_records(history_file, history_list);
    read_records(bookmark_file, bookmark_list);
    load_downloads();

    data_manager = webkit_website_data_manager_new(
        "base-data-directory", base_dir.c_str(),
        "base-cache-directory", (base_dir + "/cache").c_str(),
        NULL
    );

    webkit_website_data_manager_set_network_proxy_settings(data_manager, WEBKIT_NETWORK_PROXY_MODE_NO_PROXY, NULL);

    cookie_manager = webkit_website_data_manager_get_cookie_manager(data_manager);
    webkit_cookie_manager_set_persistent_storage(
        cookie_manager,
        (base_dir + "/cookies/cookies.sqlite").c_str(),
        WEBKIT_COOKIE_PERSISTENT_STORAGE_SQLITE
    );
    apply_privacy_and_security_policies();

    mkdir((base_dir + "/favicons").c_str(), 0777);
    web_context = webkit_web_context_new_with_website_data_manager(data_manager);
    webkit_web_context_set_favicon_database_directory(web_context, (base_dir + "/favicons").c_str());
    g_signal_connect(web_context, "download-started", G_CALLBACK(on_download_started), NULL);

    user_content_manager = webkit_user_content_manager_new();
    webkit_user_content_manager_register_script_message_handler(user_content_manager, "wb_page_loaded");
    g_signal_connect(user_content_manager, "script-message-received::wb_page_loaded",
        G_CALLBACK(+[](WebKitUserContentManager*, WebKitJavascriptResult* js_result, gpointer) {
            if (!js_result || !notebook || !GTK_IS_NOTEBOOK(notebook)) return;
            JSCValue* val = webkit_javascript_result_get_js_value(js_result);
            if (!val || !jsc_value_is_string(val)) return;
            gchar* loaded_uri = jsc_value_to_string(val);
            if (!loaded_uri) return;
            const int n = gtk_notebook_get_n_pages(GTK_NOTEBOOK(notebook));
            for (int i = 0; i < n; ++i) {
                GtkWidget* page = gtk_notebook_get_nth_page(GTK_NOTEBOOK(notebook), i);
                GtkWidget* wv_widget = get_webview_from_page(page);
                if (!wv_widget || !WEBKIT_IS_WEB_VIEW(wv_widget)) continue;
                WebKitWebView* wv = WEBKIT_WEB_VIEW(wv_widget);
                const gchar* cur_uri = webkit_web_view_get_uri(wv);
                if (cur_uri && strcmp(cur_uri, loaded_uri) == 0 && !is_webview_visually_finished(wv)) {
                    set_webview_visually_finished(wv, true);
                    set_webview_max_progress(wv, 1.0);
                    if (wv_widget == get_current_webview()) {
                        if (progress_bar && GTK_IS_WIDGET(progress_bar)) {
                            gtk_progress_bar_set_fraction(GTK_PROGRESS_BAR(progress_bar), 1.0);
                            g_timeout_add(220, hide_progress_if_idle, NULL);
                        }
                        update_status_text(tr_full("完成", "完成", "Done"), TRUE);
                    }
                }
            }
            g_free(loaded_uri);
        }), NULL);
    {
        const char* load_hook_js =
            "window.addEventListener('load', function() {"
            "  try { window.webkit.messageHandlers.wb_page_loaded.postMessage(location.href); } catch(e) {}"
            "}, { once: true });";
        WebKitUserScript* load_script = webkit_user_script_new(
            load_hook_js,
            WEBKIT_USER_CONTENT_INJECT_TOP_FRAME,
            WEBKIT_USER_SCRIPT_INJECT_AT_DOCUMENT_END,
            NULL, NULL);
        webkit_user_content_manager_add_script(user_content_manager, load_script);
        webkit_user_script_unref(load_script);
    }
    content_filter_store = webkit_user_content_filter_store_new((base_dir + "/adblock").c_str());
    load_adblock_rules();
    apply_adblock_rules();

    GtkWidget* vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    main_window = window;

    // 1. Firefox 风格顶部标签栏
    GtkWidget* tab_strip_scroll = gtk_scrolled_window_new(NULL, NULL);
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(tab_strip_scroll), GTK_POLICY_AUTOMATIC, GTK_POLICY_NEVER);
    gtk_style_context_add_class(gtk_widget_get_style_context(tab_strip_scroll), "tab-strip");
    tab_bar_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 4);
    gtk_container_add(GTK_CONTAINER(tab_strip_scroll), tab_bar_box);
    gtk_box_pack_start(GTK_BOX(vbox), tab_strip_scroll, FALSE, FALSE, 0);

    // 2. Firefox 风格导航栏：后退 前进 刷新/停止 | 地址栏 | 收藏 | 下载 | ☰ 菜单
    GtkWidget* toolbar = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 6);
    gtk_style_context_add_class(gtk_widget_get_style_context(toolbar), "nav-toolbar");

    btn_back = gtk_button_new();
    gtk_container_add(GTK_CONTAINER(btn_back), create_vector_icon(ICON_BACK, 18));
    btn_forward = gtk_button_new();
    gtk_container_add(GTK_CONTAINER(btn_forward), create_vector_icon(ICON_FORWARD, 18));
    btn_refresh = gtk_button_new();
    icon_refresh = create_vector_icon(ICON_REFRESH, 18);
    gtk_container_add(GTK_CONTAINER(btn_refresh), icon_refresh);
    gtk_button_set_relief(GTK_BUTTON(btn_back), GTK_RELIEF_NONE);
    gtk_button_set_relief(GTK_BUTTON(btn_forward), GTK_RELIEF_NONE);
    gtk_button_set_relief(GTK_BUTTON(btn_refresh), GTK_RELIEF_NONE);
    gtk_widget_set_can_focus(btn_back, FALSE);
    gtk_widget_set_can_focus(btn_forward, FALSE);
    gtk_widget_set_can_focus(btn_refresh, FALSE);
    gtk_style_context_add_class(gtk_widget_get_style_context(btn_back), "flat");
    gtk_style_context_add_class(gtk_widget_get_style_context(btn_forward), "flat");
    gtk_style_context_add_class(gtk_widget_get_style_context(btn_refresh), "flat");

    url_entry = gtk_entry_new();
    gtk_widget_set_hexpand(url_entry, TRUE);
    gtk_style_context_add_class(gtk_widget_get_style_context(url_entry), "omnibox");

    btn_bookmark = gtk_button_new();
    icon_bookmark = create_vector_icon(ICON_STAR_OUTLINE, 18);
    gtk_container_add(GTK_CONTAINER(btn_bookmark), icon_bookmark);
    gtk_button_set_relief(GTK_BUTTON(btn_bookmark), GTK_RELIEF_NONE);
    gtk_widget_set_can_focus(btn_bookmark, FALSE);
    gtk_style_context_add_class(gtk_widget_get_style_context(btn_bookmark), "flat");

    btn_bookmarks_menu = gtk_menu_button_new();
    gtk_container_add(GTK_CONTAINER(btn_bookmarks_menu), create_vector_icon(ICON_BOOKMARKS, 18));
    gtk_button_set_relief(GTK_BUTTON(btn_bookmarks_menu), GTK_RELIEF_NONE);
    gtk_widget_set_can_focus(btn_bookmarks_menu, FALSE);
    gtk_style_context_add_class(gtk_widget_get_style_context(btn_bookmarks_menu), "flat");

    btn_downloads = gtk_menu_button_new();
    GtkWidget* dl_btn_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 4);
    gtk_box_pack_start(GTK_BOX(dl_btn_box), create_vector_icon(ICON_DOWNLOAD, 18), FALSE, FALSE, 0);
    lbl_downloads_badge = gtk_label_new("");
    gtk_widget_set_no_show_all(lbl_downloads_badge, TRUE);
    gtk_box_pack_start(GTK_BOX(dl_btn_box), lbl_downloads_badge, FALSE, FALSE, 0);
    gtk_container_add(GTK_CONTAINER(btn_downloads), dl_btn_box);
    gtk_button_set_relief(GTK_BUTTON(btn_downloads), GTK_RELIEF_NONE);
    gtk_widget_set_can_focus(btn_downloads, FALSE);
    gtk_style_context_add_class(gtk_widget_get_style_context(btn_downloads), "flat");

    btn_menu = gtk_menu_button_new();
    gtk_container_add(GTK_CONTAINER(btn_menu), create_vector_icon(ICON_MENU, 18));
    gtk_button_set_relief(GTK_BUTTON(btn_menu), GTK_RELIEF_NONE);
    gtk_widget_set_can_focus(btn_menu, FALSE);
    gtk_style_context_add_class(gtk_widget_get_style_context(btn_menu), "flat");

    g_signal_connect(url_entry, "focus-in-event", G_CALLBACK(on_url_entry_focus_in), NULL);
    g_signal_connect(url_entry, "focus-out-event", G_CALLBACK(on_url_entry_focus_out), NULL);
    g_signal_connect(url_entry, "populate-popup", G_CALLBACK(on_url_entry_populate_popup), NULL);

    gtk_box_pack_start(GTK_BOX(toolbar), btn_back, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(toolbar), btn_forward, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(toolbar), btn_refresh, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(toolbar), url_entry, TRUE, TRUE, 4);
    gtk_box_pack_start(GTK_BOX(toolbar), btn_bookmark, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(toolbar), btn_bookmarks_menu, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(toolbar), btn_downloads, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(toolbar), btn_menu, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(vbox), toolbar, FALSE, FALSE, 0);

    // 下载弹出面板（最近下载 + 打开下载管理）
    dl_popover = gtk_popover_new(btn_downloads);
    gtk_widget_set_size_request(dl_popover, 360, 320);
    GtkWidget* dl_pop_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 6);
    gtk_container_set_border_width(GTK_CONTAINER(dl_pop_box), 10);
    GtkWidget* dl_pop_header = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 6);
    dl_popover_title = gtk_label_new(tr_full("最近下载", "最近下載", "Recent downloads"));
    gtk_widget_set_halign(dl_popover_title, GTK_ALIGN_START);
    dl_popover_manage_btn = gtk_button_new_with_label(tr_full("打开下载管理", "開啟下載管理", "Open Downloads Manager"));
    gtk_widget_set_can_focus(dl_popover_manage_btn, FALSE);
    gtk_box_pack_start(GTK_BOX(dl_pop_header), dl_popover_title, TRUE, TRUE, 0);
    gtk_box_pack_end(GTK_BOX(dl_pop_header), dl_popover_manage_btn, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(dl_pop_box), dl_pop_header, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(dl_pop_box), gtk_separator_new(GTK_ORIENTATION_HORIZONTAL), FALSE, FALSE, 0);

    GtkWidget* dl_pop_scroll = gtk_scrolled_window_new(NULL, NULL);
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(dl_pop_scroll), GTK_POLICY_NEVER, GTK_POLICY_AUTOMATIC);
    gtk_widget_set_vexpand(dl_pop_scroll, TRUE);
    dl_popover_listbox = gtk_list_box_new();
    gtk_list_box_set_selection_mode(GTK_LIST_BOX(dl_popover_listbox), GTK_SELECTION_NONE);
    gtk_container_add(GTK_CONTAINER(dl_pop_scroll), dl_popover_listbox);
    gtk_box_pack_start(GTK_BOX(dl_pop_box), dl_pop_scroll, TRUE, TRUE, 0);
    gtk_widget_show_all(dl_pop_box);
    gtk_container_add(GTK_CONTAINER(dl_popover), dl_pop_box);
    gtk_menu_button_set_popover(GTK_MENU_BUTTON(btn_downloads), dl_popover);

    g_signal_connect(dl_popover, "show", G_CALLBACK(+[](GtkWidget*, gpointer) {
        if (dl_popover_listbox) rebuild_download_list_ui(dl_popover_listbox, false);
    }), NULL);
    g_signal_connect(dl_popover, "hide", G_CALLBACK(+[](GtkWidget*, gpointer) {
        if (active_download_listbox == dl_popover_listbox) {
            if (preferences_page && pref_dl_listbox && pref_current_row == 7) {
                rebuild_download_list_ui(pref_dl_listbox, record_multi_mode);
            } else {
                clear_download_row_widgets();
                active_download_listbox = nullptr;
                active_download_multi_mode = false;
            }
        }
    }), NULL);
    g_signal_connect(dl_popover_manage_btn, "clicked", G_CALLBACK(+[](GtkWidget*, gpointer) {
        if (dl_popover) gtk_widget_hide(dl_popover);
        open_preferences_tab("downloads_list");
    }), NULL);

    // 书签弹出面板
    bm_popover = gtk_popover_new(btn_bookmarks_menu);
    gtk_widget_set_size_request(bm_popover, 340, 320);
    GtkWidget* bm_pop_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 6);
    gtk_container_set_border_width(GTK_CONTAINER(bm_pop_box), 10);
    GtkWidget* bm_pop_header = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 6);
    bm_popover_title = gtk_label_new(tr_full("书签", "書籤", "Bookmarks"));
    gtk_widget_set_halign(bm_popover_title, GTK_ALIGN_START);
    bm_popover_manage_btn = gtk_button_new_with_label(tr_full("打开书签管理", "開啟書籤管理", "Open Bookmarks Manager"));
    gtk_box_pack_start(GTK_BOX(bm_pop_header), bm_popover_title, TRUE, TRUE, 0);
    gtk_box_pack_end(GTK_BOX(bm_pop_header), bm_popover_manage_btn, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(bm_pop_box), bm_pop_header, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(bm_pop_box), gtk_separator_new(GTK_ORIENTATION_HORIZONTAL), FALSE, FALSE, 0);

    GtkWidget* bm_pop_scroll = gtk_scrolled_window_new(NULL, NULL);
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(bm_pop_scroll), GTK_POLICY_NEVER, GTK_POLICY_AUTOMATIC);
    gtk_widget_set_vexpand(bm_pop_scroll, TRUE);
    bm_popover_listbox = gtk_list_box_new();
    gtk_list_box_set_selection_mode(GTK_LIST_BOX(bm_popover_listbox), GTK_SELECTION_NONE);
    gtk_container_add(GTK_CONTAINER(bm_pop_scroll), bm_popover_listbox);
    gtk_box_pack_start(GTK_BOX(bm_pop_box), bm_pop_scroll, TRUE, TRUE, 0);
    gtk_widget_show_all(bm_pop_box);
    gtk_container_add(GTK_CONTAINER(bm_popover), bm_pop_box);
    gtk_menu_button_set_popover(GTK_MENU_BUTTON(btn_bookmarks_menu), bm_popover);

    g_signal_connect(bm_popover, "show", G_CALLBACK(+[](GtkWidget*, gpointer) {
        rebuild_bookmark_popover_ui();
    }), NULL);
    g_signal_connect(bm_popover_manage_btn, "clicked", G_CALLBACK(+[](GtkWidget*, gpointer) {
        if (bm_popover) gtk_widget_hide(bm_popover);
        open_preferences_tab("bookmarks");
    }), NULL);



    // Firefox 汉堡菜单弹出面板
    menu_popover = gtk_popover_new(btn_menu);
    GtkWidget* menu_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 2);
    gtk_container_set_border_width(GTK_CONTAINER(menu_box), 8);

    menu_history_btn = create_menu_item_button(tr_full("历史记录", "歷史記錄", "History"));
    menu_bookmark_btn = create_menu_item_button(tr_full("书签管理", "書籤管理", "Bookmarks"));
    menu_downloads_btn = create_menu_item_button(tr_full("下载管理", "下載管理", "Downloads"));
    menu_cookies_btn = create_menu_item_button("Cookies");
    menu_settings_btn = create_menu_item_button(tr_full("设置", "設定", "Settings"));

    gtk_box_pack_start(GTK_BOX(menu_box), menu_history_btn, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(menu_box), menu_bookmark_btn, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(menu_box), menu_downloads_btn, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(menu_box), menu_cookies_btn, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(menu_box), gtk_separator_new(GTK_ORIENTATION_HORIZONTAL), FALSE, FALSE, 4);
    gtk_box_pack_start(GTK_BOX(menu_box), menu_settings_btn, FALSE, FALSE, 0);

    gtk_widget_show_all(menu_box);
    gtk_container_add(GTK_CONTAINER(menu_popover), menu_box);
    gtk_menu_button_set_popover(GTK_MENU_BUTTON(btn_menu), menu_popover);


    g_signal_connect(menu_history_btn, "clicked", G_CALLBACK(+[](GtkWidget*, gpointer) {
        open_preferences_tab("history");
    }), NULL);
    g_signal_connect(menu_bookmark_btn, "clicked", G_CALLBACK(+[](GtkWidget*, gpointer) {
        open_preferences_tab("bookmarks");
    }), NULL);
    g_signal_connect(menu_downloads_btn, "clicked", G_CALLBACK(+[](GtkWidget*, gpointer) {
        open_preferences_tab("downloads_list");
    }), NULL);
    g_signal_connect(menu_cookies_btn, "clicked", G_CALLBACK(+[](GtkWidget*, gpointer) {
        open_preferences_tab("cookies");
    }), NULL);
    g_signal_connect(menu_settings_btn, "clicked", G_CALLBACK(+[](GtkWidget*, gpointer) {
        open_preferences_tab("general");
    }), NULL);

    // 3. 进度条
    progress_bar = gtk_progress_bar_new();
    gtk_widget_set_size_request(progress_bar, -1, 3);
    gtk_widget_hide(progress_bar);
    gtk_box_pack_start(GTK_BOX(vbox), progress_bar, FALSE, FALSE, 0);

    // 4. 主内容容器
    GtkWidget* overlay = gtk_overlay_new();
    gtk_box_pack_start(GTK_BOX(vbox), overlay, TRUE, TRUE, 0);

    notebook = gtk_notebook_new();
    gtk_notebook_set_show_tabs(GTK_NOTEBOOK(notebook), FALSE);
    gtk_notebook_set_show_border(GTK_NOTEBOOK(notebook), FALSE);
    gtk_container_add(GTK_CONTAINER(overlay), notebook);

    status_overlay = gtk_label_new("");
    gtk_widget_set_halign(status_overlay, GTK_ALIGN_START);
    gtk_widget_set_valign(status_overlay, GTK_ALIGN_END);

    GtkCssProvider* css = gtk_css_provider_new();
    gtk_css_provider_load_from_data(css,
        "label { background-color: rgba(50, 50, 50, 0.8); color: white; padding: 4px 8px; border-radius: 4px; font-size: 12px; }", -1, NULL);
    gtk_style_context_add_provider(gtk_widget_get_style_context(status_overlay), GTK_STYLE_PROVIDER(css), GTK_STYLE_PROVIDER_PRIORITY_USER);
    g_object_unref(css);

    gtk_widget_set_margin_start(status_overlay, 10);
    gtk_widget_set_margin_bottom(status_overlay, 10);
    gtk_overlay_add_overlay(GTK_OVERLAY(overlay), status_overlay);
    gtk_widget_hide(status_overlay);

    gtk_container_add(GTK_CONTAINER(window), vbox);

    g_signal_connect(url_entry, "activate", G_CALLBACK(on_url_activate), NULL);
    g_signal_connect(btn_back, "clicked", G_CALLBACK(on_back), NULL);
    g_signal_connect(btn_forward, "clicked", G_CALLBACK(on_forward), NULL);
    g_signal_connect(btn_refresh, "clicked", G_CALLBACK(on_refresh), NULL);
    g_signal_connect(btn_bookmark, "clicked", G_CALLBACK(on_bookmark), NULL);
    g_signal_connect(notebook, "switch-page", G_CALLBACK(on_switch_page), NULL);
    g_signal_connect(window, "size-allocate", G_CALLBACK(on_window_size_allocate), NULL);

    apply_ui_language();
    update_download_button_badge();
    add_tab(get_home_url().c_str());
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
    if (g_file_test(app_icon_path.c_str(), G_FILE_TEST_EXISTS)) {
        gtk_window_set_icon_from_file(GTK_WINDOW(window), app_icon_path.c_str(), NULL);
    }

    g_signal_connect(window, "destroy", G_CALLBACK(on_main_window_destroy), NULL);
    build_ui(window);
    gtk_widget_show_all(window);
    gtk_main();
    return 0;
}
