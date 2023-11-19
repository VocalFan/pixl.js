#include "app_chameleon.h"
#include "chameleon_scene.h"

#include "mini_app_launcher.h"
#include "mini_app_registry.h"

#include "amiibo_helper.h"
#include "nrf_log.h"
#include "nrf_log_ctrl.h"

#include "i18n/language.h"
#include "settings.h"

#include "mui_icons.h"
#include "tag_helper.h"

#include "fds_utils.h"

typedef enum {
    CHAMELEON_MENU_BACK,
    CHAMELEON_MENU_FILE,
} chameleon_menu_item_t;

int32_t chameleon_scene_menu_card_data_file_save_from_file(app_chameleon_t *app, const char *file_name){

}

int32_t chameleon_scene_menu_card_data_file_save_reload(app_chameleon_t *app) {
    vfs_dir_t dir;
    vfs_obj_t obj;

    vfs_driver_t *p_driver = vfs_get_driver(VFS_DRIVE_EXT);
    int32_t res = p_driver->open_dir(CHELEMEON_DUMP_FOLDER, &dir);
    if (res < 0) {
        return -1;
    }

    while (p_driver->read_dir(&dir, &obj) == VFS_OK) {
        if (obj.type == VFS_TYPE_REG) {
            mui_list_view_add_item(app->p_list_view, ICON_FILE, obj.name, (void *)CHAMELEON_MENU_FILE);
        }
    }
    p_driver->close_dir(&dir);
    return 0;
}

void chameleon_scene_menu_card_data_file_save_on_event(mui_list_view_event_t event, mui_list_view_t *p_list_view,
                                                         mui_list_item_t *p_item) {
    app_chameleon_t *app = p_list_view->user_data;
    chameleon_menu_item_t item = (chameleon_menu_item_t)p_item->user_data;
    switch (item) {
    case CHAMELEON_MENU_FILE: {
        uint8_t slot = tag_emulation_get_slot();

        mui_scene_dispatcher_previous_scene(app->p_scene_dispatcher);
    } break;

    case CHAMELEON_MENU_BACK:
        mui_scene_dispatcher_previous_scene(app->p_scene_dispatcher);
        break;
    }
}

void chameleon_scene_menu_card_data_file_save_on_enter(void *user_data) {
    app_chameleon_t *app = user_data;

    chameleon_scene_menu_card_data_file_save_reload(app);
    mui_list_view_add_item(app->p_list_view, ICON_BACK, getLangString(_L_MAIN_RETURN), (void *)CHAMELEON_MENU_BACK);

    mui_list_view_set_selected_cb(app->p_list_view, chameleon_scene_menu_card_data_file_save_on_event);
    mui_view_dispatcher_switch_to_view(app->p_view_dispatcher, CHAMELEON_VIEW_ID_LIST);
}

void chameleon_scene_menu_card_data_file_save_on_exit(void *user_data) {
    app_chameleon_t *app = user_data;
    mui_list_view_clear_items(app->p_list_view);
}