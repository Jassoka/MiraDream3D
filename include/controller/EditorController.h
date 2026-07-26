//
// Created by jassoka on 7/26/26.
//

#ifndef MIRADREAM3D_EDITORCONTROLLER_H
#define MIRADREAM3D_EDITORCONTROLLER_H
#include <QObject>


class RenderController;
enum class EditorTool;
/**
 * @brief Controller class for user editing
 */
class EditorController: public QObject
{
public:
    explicit EditorController(QObject *parent, RenderController *render_controller):
        QObject(parent),
        mRenderController(render_controller) {}
public slots:
    void setTool(EditorTool tool);
private:
    EditorTool mCurrentTool;
    RenderController *mRenderController;
};

#endif //MIRADREAM3D_EDITORCONTROLLER_H
