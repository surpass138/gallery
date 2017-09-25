#include "gallerymiddlewidgets.h"
#include <QDir>
#include <qdebug.h>
#include <QApplication>
#include <QDirIterator>
#include <QDateTime>

#include "global_value.h"

GalleryMiddleWidgets::GalleryMiddleWidgets(QWidget *parent):BaseWidget(parent)
{
    // Set background color.
    setObjectName("GalleryMiddleWidgets");
    setStyleSheet("#GalleryMiddleWidgets{background:rgb(43,45,51)}");

    initLayout();
    initConnection();
}

void GalleryMiddleWidgets::initLayout()
{
    m_stackedMainLyout = new QStackedLayout(this);

    m_emptyImgWid = new EmptyImagesWidget(this);
    m_thumbImgWid = new ThumbImageWidget(this);
    m_viewerWid = new ImageViewerWidget(this);

    m_stackedMainLyout->addWidget(m_emptyImgWid);
    m_stackedMainLyout->addWidget(m_thumbImgWid);
    m_stackedMainLyout->addWidget(m_viewerWid);
    m_stackedMainLyout->setCurrentWidget(m_thumbImgWid);

    setLayout(m_stackedMainLyout);
}

void GalleryMiddleWidgets::initConnection()
{
    connect(this,SIGNAL(imageEmpty()),this,SLOT(slot_showEmptyImageTip()));
    connect(this,SIGNAL(imageItemClick(QString,QImage*)),this,SLOT(slot_showImageViewer(QString,QImage*)));
    connect(this,SIGNAL(imagesResChanged()),this,SLOT(slot_onImagesResChanged()));
}

void GalleryMiddleWidgets::slot_showEmptyImageTip()
{
    emit viewerResChanged("");
    m_stackedMainLyout->setCurrentWidget(m_emptyImgWid);
}

void GalleryMiddleWidgets::slot_showImageViewer(QString imagePath, QImage *image)
{
    m_stackedMainLyout->setCurrentWidget(m_viewerWid);
    m_viewerWid->updateRes(imagePath,image);
}

void GalleryMiddleWidgets::slot_onImagesResChanged()
{
    QMap<QString,QImage*> &imagesRes = mainWindow->getGalleryWidget()->getImagesRes();
    if(imagesRes.size()>0){
        if(m_stackedMainLyout->currentWidget()==m_emptyImgWid){
            m_stackedMainLyout->setCurrentWidget(m_thumbImgWid);
        }
        emit m_thumbImgWid->imagesResChanged();
        emit m_viewerWid->imagesResChanged(m_stackedMainLyout->currentWidget()==m_viewerWid);
    }else{
        emit imageEmpty();
    }
}

void GalleryMiddleWidgets::leaveViewerMode()
{
    m_stackedMainLyout->setCurrentWidget(m_thumbImgWid);
}


