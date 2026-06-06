//
// Created by jassoka on 6/5/26.
//

#include "RenderDocHelper.hpp"
#include <QLibrary>
#include <QDebug>

// On crée réellement la variable globale ici
RENDERDOC_API_1_1_2* rdoc_api = nullptr;

void initRenderDoc() {
    // Ouverture de la librairie
#ifdef Q_OS_WIN
    QLibrary rdocLib("renderdoc.dll");
#else
    QLibrary rdocLib("librenderdoc.so");
#endif

    if (!rdocLib.load()) {
        qDebug() << "[INFO] RenderDoc non détecté. Exécution normale.";
        return;
    }

    // 2. On cherche la fonction "RENDERDOC_GetAPI" à l'intérieur de la DLL
    pRENDERDOC_GetAPI RENDERDOC_GetAPI =
        (pRENDERDOC_GetAPI)rdocLib.resolve("RENDERDOC_GetAPI");

    if (!RENDERDOC_GetAPI) {
        qDebug() << "[ERREUR] DLL RenderDoc trouvée, mais corrompue ou obsolète.";
        return;
    }

    // 3. On branche notre pointeur rdoc_api !
    RENDERDOC_GetAPI(eRENDERDOC_API_Version_1_1_2, (void **)&rdoc_api);
    qDebug() << "[SUCCÈS] RenderDoc API connectée ! Les macros de capture sont actives.";
}