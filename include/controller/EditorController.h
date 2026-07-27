//
// Created by jassoka on 7/26/26.
//

#ifndef MIRADREAM3D_EDITORCONTROLLER_H
#define MIRADREAM3D_EDITORCONTROLLER_H
#include <QObject>


class RenderWidget;
class RenderController;
enum class EditorTool;
/**
 * @brief Controller class for user editing
 */
class EditorController: public QObject
{
public:
    explicit EditorController(QObject *parent, RenderController *render_controller, RenderWidget *render_widget);
public slots:
    void setTool(EditorTool tool);
    void onMouseDrag(int dx, int dy, Qt::MouseButtons buttons);
    void onMouseScroll(int deltaX, int deltaY);
    void onMousePress(Qt::MouseButton button, int localX, int localY);
    void onMouseRelease(Qt::MouseButton button) const;
    void refresh();
    signals:
    void teleportMouseToCenter();
    void hideCursor();

private:
    bool canRotateCamera(Qt::MouseButton button) const;
    bool canRotateCamera(Qt::MouseButtons buttons) const;
    EditorTool mCurrentTool;
    RenderController *mRenderController;
    RenderWidget *mRenderWidget;


    int mMouseDragRotateX = 0; // variable qui stocke la valeur de rotation
    int mMouseDragRotateY = 0;

    int mMouseDragTranslateX = 0; // variable qui stocke la valeur de translation
    int mMouseDragTranslateY = 0;

    int mMouseScroll = 0;


    float mMouseRotateSensitivity = 0.0035f;
    float mMouseTranslateSensitivity = 0.0035f;
    float mScrollSensitivity = 0.00035f;
};

#endif //MIRADREAM3D_EDITORCONTROLLER_H
