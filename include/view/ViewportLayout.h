//
// Created by jassoka on 8/2/26.
//

#ifndef MIRADREAM3D_VIEWPORTLAYOUT_H
#define MIRADREAM3D_VIEWPORTLAYOUT_H
#include <QWidget>


enum class ViewportMode;
class QActionGroup;
class QAction;

class ViewportLayout: public QWidget
{
    Q_OBJECT
public:
    explicit ViewportLayout(QWidget *parent);
    signals:
    void clickedViewportMode(ViewportMode mode);
public slots:
    void onViewportChange(ViewportMode mode) const;
protected:
    void mousePressEvent(QMouseEvent *event) override;
private:
    void initActions();
    QActionGroup* mViewportModes = nullptr;
    QAction* mMaterial = nullptr;
    QAction* mWireFrame = nullptr;
    QAction* mSolid = nullptr;
};


#endif //MIRADREAM3D_VIEWPORTLAYOUT_H
