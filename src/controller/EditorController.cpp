//
// Created by jassoka on 7/26/26.
//
#include "controller/EditorController.h"

#include "types.h"
#include "controller/RenderController.h"
#include "view/RenderWidget.h"

EditorController::EditorController(QObject* parent, RenderController* render_controller, RenderWidget* render_widget)
:
    QObject(parent),
    mRenderController(render_controller),
    mRenderWidget(render_widget)
{
    setTool(EditorTool::NAVIGATE);
    connect(mRenderWidget, &RenderWidget::mouseDrag, this, &EditorController::onMouseDrag);
    connect(mRenderWidget, &RenderWidget::mouseScroll, this, &EditorController::onMouseScroll);
    connect(mRenderWidget, &RenderWidget::mousePress, this, &EditorController::onMousePress);
    connect(mRenderWidget, &RenderWidget::mouseRelease, this, &EditorController::onMouseRelease);
    connect(mRenderWidget, &RenderWidget::refresh, this, &EditorController::refresh);
}

void EditorController::setTool(const EditorTool tool)
{
    mCurrentTool = tool;
    mRenderController->toolChanged(tool);
    switch (tool)
    {
    case EditorTool::NAVIGATE:
        mRenderWidget->setCursor(Qt::OpenHandCursor); break;
    case EditorTool::SELECTION:
        mRenderWidget->setCursor(Qt::ArrowCursor); break;
    }
}

bool EditorController::canRotateCamera(const Qt::MouseButton button) const
{
    return button == Qt::MiddleButton || (button == Qt::LeftButton && mCurrentTool == EditorTool::NAVIGATE);
}
bool EditorController::canRotateCamera(const Qt::MouseButtons buttons) const
{
    return buttons & Qt::MiddleButton || (buttons & Qt::LeftButton && mCurrentTool == EditorTool::NAVIGATE);
}

void EditorController::onMouseDrag(const int dx, const int dy, Qt::MouseButtons buttons)
{
    buttons &= mHeldButtons;
    if (canRotateCamera(buttons) ) {
        mMouseDragRotateX += dx;
        mMouseDragRotateY += dy;
        mRenderWidget->teleportMouseToCenter();
    }
    if (buttons & Qt::RightButton) {
        mMouseDragTranslateX += dx;
        mMouseDragTranslateY += dy;
        mRenderWidget->teleportMouseToCenter();
    }
}

void EditorController::onMouseScroll(int deltaX, int deltaY)
{
    mMouseScroll += deltaX + 2*deltaY;
}

void EditorController::onMousePress(Qt::MouseButton button, int localX, int localY)
{
    if (canRotateCamera(button) || button == Qt::RightButton) {
        mRenderWidget->grabMouse();
        mRenderWidget->initMouseDrag();

        const auto prevShape = mRenderWidget->cursor().shape();
        if (prevShape != Qt::BlankCursor) mMouseStyleBuffer = prevShape;
        mRenderWidget->setCursor(Qt::BlankCursor); // Nice UX touch: change cursor when dragging
        if (button == Qt::RightButton)
            mMouseDragTranslateX = mMouseDragTranslateY = 0;
        else
            mMouseDragRotateX = mMouseDragRotateY = 0;
    }
    if (button == Qt::LeftButton && mCurrentTool == EditorTool::SELECTION)
    {
        const double scale = mRenderWidget->devicePixelRatioF();
        mRenderController->pickFromScreen(localX*scale, localY*scale);
    }
    mHeldButtons |= button;
}

void EditorController::onMouseRelease(const Qt::MouseButton button)
{
    if (canRotateCamera(button) || button == Qt::RightButton) {
        mRenderWidget->releaseMouse();
        mRenderWidget->setCursor(mMouseStyleBuffer); // Nice UX touch: change cursor when dragging
    }
    mHeldButtons &= ~button;
}

void EditorController::refresh()
{
    if (mMouseDragRotateX != 0 || mMouseDragRotateY != 0) // Engager rotation de la caméra
    {
        float dPhi   = -mMouseDragRotateX * mMouseRotateSensitivity;
        float dTheta = -mMouseDragRotateY * mMouseRotateSensitivity;
        mMouseDragRotateX = mMouseDragRotateY = 0;

        mRenderController->rotateAroundAnchor(dPhi, dTheta);
    }
    if (mMouseDragTranslateX != 0 || mMouseDragTranslateY != 0) // Engager translation de la caméra
    {
        float dx = mMouseDragTranslateX * mMouseTranslateSensitivity;
        float dy = -mMouseDragTranslateY * mMouseTranslateSensitivity;
        mMouseDragTranslateX = mMouseDragTranslateY = 0;

        mRenderController->cameraStrafe(dx, dy);
    }

    if (mMouseScroll != 0)
    {
        mRenderController->cameraZoom(exp(-mMouseScroll*mScrollSensitivity));
        mMouseScroll = 0;
    }
}
