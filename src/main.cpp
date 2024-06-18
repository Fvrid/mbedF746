#include <mbed.h> 
#include <threadLvgl.h> 
#include <cstdio> 

// Permet de définir les broches RX et TX
#define RX_PIN D1 
#define TX_PIN D0 

// Initialiser la connexion série pour le scanner de codes-barres ainsi que le thread pour la bibliothèque LVGL
BufferedSerial barcodeScanner(RX_PIN, TX_PIN, 9600); 
ThreadLvgl threadLvgl(30); 
lv_obj_t *list; 

// Gestionnaire d'événements pour le bouton de scanner
void scanner_button_event_handler(lv_event_t *e) { 
    char buffer[128]; 
    memset(buffer, 0, sizeof(buffer)); 

    while (true) { 
        // Si des données sont disponibles, lecture des données disponibles
        if (barcodeScanner.readable()) { 
            int len = barcodeScanner.read(buffer, sizeof(buffer) - 1);
            if (len > 0) { 
                buffer[len] = '\0'; 
                printf("Code scanné: %s\n", buffer); // Afficher le code scanné sur l'interface 

                // Ajouter un nouvel élément à la liste, aligner le bouton au centre, creation d'un nouveau bouton dans la liste
                lv_obj_t *list_btn = lv_list_add_btn(list, NULL, buffer); 
                lv_obj_align(list_btn, LV_ALIGN_CENTER, 0, 0); 

                // Créer un bouton pour effacer l'élément de la liste, taille, alignement, création d'un label pour le bouton, centrer le texte  
                lv_obj_t *delete_btn = lv_btn_create(list_btn);
                lv_obj_set_size(delete_btn, 50, 25);
                lv_obj_align(delete_btn, LV_ALIGN_RIGHT_MID, -10, 0); 
                lv_obj_t *delete_label = lv_label_create(delete_btn);
                lv_label_set_text(delete_label, "Effacer"); 
                lv_obj_center(delete_label); 

                // Ajouter un événement pour le bouton Effacer
                lv_obj_add_event_cb(delete_btn, [](lv_event_t *e) { 
                    lv_obj_t *btn = lv_event_get_target(e); 
                    lv_obj_del(lv_obj_get_parent(btn)); 
                }, LV_EVENT_CLICKED, NULL);

                memset(buffer, 0, sizeof(buffer)); // allouer désallouer
                break; 
            }
        }
        ThisThread::sleep_for(10ms); 
    }
}

int main() { 
    
    threadLvgl.lock(); 

    // Définir la couleur de fond de l'interface en bleu
    lv_obj_set_style_bg_color(lv_scr_act(), lv_color_hex(0x0066FF), 0); 

    // Créer un label pour le titre, alignement
    lv_obj_t *labelTitle = lv_label_create(lv_scr_act()); 
    lv_label_set_text(labelTitle, "GESTION D'INVENTAIRE"); 
    lv_obj_align(labelTitle, LV_ALIGN_TOP_MID, 0, 10); 

    // Créer un bouton pour scanner, couleur, alignement, centrée
    lv_obj_t *btn = lv_btn_create(lv_scr_act()); 
    lv_obj_align(btn, LV_ALIGN_CENTER, 0, -75); 
    lv_obj_set_style_bg_color(btn, lv_color_hex(0xFF0000), 0); 
    lv_obj_add_event_cb(btn, scanner_button_event_handler, LV_EVENT_CLICKED, NULL); 
    lv_obj_t *btnLabel = lv_label_create(btn); 
    lv_label_set_text(btnLabel, "Scanner");
    lv_obj_center(btnLabel); 

    // Créer une liste pour afficher les codes-barres scannés, taille, création liste 
    list = lv_list_create(lv_scr_act()); 
    lv_obj_set_size(list, 400, 150); 
    lv_obj_align(list, LV_ALIGN_CENTER, 0, 40);

    threadLvgl.unlock();

    while (true) {
        ThisThread::sleep_for(10ms); 
    }
}








