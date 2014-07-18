/****************************************************************************
**
** Copyright (C) 2014 Dinu SV.
** (contact: mail@dinusv.com)
** This file is part of Live CV application.
**
** GNU General Public License Usage
** 
** This file may be used under the terms of the GNU General Public License 
** version 3.0 as published by the Free Software Foundation and appearing 
** in the file LICENSE.GPL included in the packaging of this file.  Please 
** review the following information to ensure the GNU General Public License 
** version 3.0 requirements will be met: http://www.gnu.org/copyleft/gpl.html.
**
****************************************************************************/

#include "QMatRead.hpp"
#include <QSGTexture>
#include <QOpenGLFramebufferObject>
#include <QOpenGLFramebufferObjectFormat>
#include <QOpenGLPaintDevice>
#include <QQuickWindow>

#include <QPainter>

using namespace cv;

QMatReadNode::QMatReadNode(QQuickWindow *window)
    : m_fbo(0)
    , m_texture(0)
    , m_window(window)
    , m_painter(new QPainter)
    , m_paintDevice(0){

}

QMatReadNode::~QMatReadNode(){
    delete m_texture;
    delete m_fbo;
    delete m_painter;
    delete m_paintDevice;
}

void QMatReadNode::render(QMat *image, const QFont &font, const QColor& color, int numberWidth, bool equalAspectRatio){
    QSize size = rect().size().toSize();
    if (!m_fbo) {
        QOpenGLFramebufferObjectFormat format;
        format.setAttachment(QOpenGLFramebufferObject::CombinedDepthStencil);
        m_fbo     = new QOpenGLFramebufferObject(size, format);
        m_texture = m_window->createTextureFromId(m_fbo->texture(), size, QQuickWindow::TextureHasAlphaChannel);
        m_paintDevice = new QOpenGLPaintDevice;
        m_paintDevice->setPaintFlipped(true);
        setTexture(m_texture);
    }

    if (image){
        m_fbo->bind();
        glClearColor(0, 0, 0, 0);
        glClear(GL_COLOR_BUFFER_BIT);

        Mat* renderSource = image->cvMat();
        int cellHeight = ( font.pixelSize() + 2 ) * renderSource->channels() + 4;
        int cellWidth = 4 + numberWidth * ( font.pixelSize() / 3 * 2 );
        if ( equalAspectRatio ){
            cellHeight = cellWidth > cellHeight ? cellWidth : cellHeight;
            cellWidth  = cellHeight;
        }
        int colCells = ceil( size.width() / cellWidth ) < renderSource->cols ? ceil( size.width() / cellWidth ) : renderSource->cols;
        int rowCells = ceil( size.height() / cellHeight ) < renderSource->rows ? ceil ( size.height() / cellHeight ) : renderSource->rows;

        m_paintDevice->setSize(size);
        m_painter->begin(m_paintDevice);
        m_painter->setRenderHints(QPainter::Antialiasing | QPainter::HighQualityAntialiasing | QPainter::TextAntialiasing );
        m_painter->setPen(QPen(color, 1));
        m_painter->setFont(font);

        switch(renderSource->depth()){
        case CV_8U : {
            for ( int i = 0; i < rowCells; ++i ) {
                uchar* p = renderSource->ptr<uchar>(i);
                for ( int j = 0; j < colCells * renderSource->channels(); j += renderSource->channels() ){
                    for ( int ch = 0; ch < renderSource->channels(); ++ch ){
                        m_painter->drawText( ( j / renderSource->channels() ) * cellWidth + 2, i * cellHeight + ( font.pixelSize() + 2 ) * ( ch + 1 ) + 2, QString::number(p[j + ch]).mid(0, numberWidth));
                    }
                }
            }
            break;
        }
        case CV_8S : {
            for ( int i = 0; i < rowCells; ++i ) {
                char* p = renderSource->ptr<char>(i);
                for ( int j = 0; j < colCells * renderSource->channels(); j += renderSource->channels() ){
                    for ( int ch = 0; ch < renderSource->channels(); ++ch ){
                        m_painter->drawText( ( j / renderSource->channels() ) * cellWidth + 2, i * cellHeight + ( font.pixelSize() + 2 ) * ( ch + 1 ) + 2, QString::number(p[j + ch]).mid(0, numberWidth));
                    }
                }
            }
            break;
        }
        case CV_16U : {
            for ( int i = 0; i < rowCells; ++i ) {
                unsigned int* p = renderSource->ptr<unsigned int>(i);
                for ( int j = 0; j < colCells * renderSource->channels(); j += renderSource->channels() ){
                    for ( int ch = 0; ch < renderSource->channels(); ++ch ){
                        m_painter->drawText( ( j / renderSource->channels() ) * cellWidth + 2, i * cellHeight + ( font.pixelSize() + 2 ) * ( ch + 1 ) + 2, QString::number(p[j + ch]).mid(0, numberWidth));
                    }
                }
            }
            break;
        }
        case CV_16S : {
            for ( int i = 0; i < rowCells; ++i ) {
                int* p = renderSource->ptr<int>(i);
                for ( int j = 0; j < colCells * renderSource->channels(); j += renderSource->channels() ){
                    for ( int ch = 0; ch < renderSource->channels(); ++ch ){
                        m_painter->drawText( ( j / renderSource->channels() ) * cellWidth + 2, i * cellHeight + ( font.pixelSize() + 2 ) * ( ch + 1 ) + 2, QString::number(p[j + ch]).mid(0, numberWidth));
                    }
                }
            }
            break;
        }
        case CV_32S : {
            for ( int i = 0; i < rowCells; ++i ) {
                long* p = renderSource->ptr<long>(i);
                for ( int j = 0; j < colCells * renderSource->channels(); j += renderSource->channels() ){
                    for ( int ch = 0; ch < renderSource->channels(); ++ch ){
                        m_painter->drawText( ( j / renderSource->channels() ) * cellWidth + 2, i * cellHeight + ( font.pixelSize() + 2 ) * ( ch + 1 ) + 2, QString::number(p[j + ch]).mid(0, numberWidth));
                    }
                }
            }
            break;
        }
        case CV_32F : {
            for ( int i = 0; i < rowCells; ++i ) {
                float* p = renderSource->ptr<float>(i);
                for ( int j = 0; j < colCells * renderSource->channels(); j += renderSource->channels() ){
                    for ( int ch = 0; ch < renderSource->channels(); ++ch ){
                        m_painter->drawText( ( j / renderSource->channels() ) * cellWidth + 2, i * cellHeight + ( font.pixelSize() + 2 ) * ( ch + 1 ) + 2, QString::number(p[j + ch]).mid(0, numberWidth));
                    }
                }
            }
            break;
        }
        case CV_64F : {
            for ( int i = 0; i < rowCells; ++i ) {
                double* p = renderSource->ptr<double>(i);
                for ( int j = 0; j < colCells * renderSource->channels(); j += renderSource->channels() ){
                    for ( int ch = 0; ch < renderSource->channels(); ++ch ){
                        m_painter->drawText( ( j / renderSource->channels() ) * cellWidth + 2, i * cellHeight + ( font.pixelSize() + 2 ) * ( ch + 1 ) + 2, QString::number(p[j + ch]).mid(0, numberWidth));
                    }
                }
            }
            break;
        }
        }

        m_painter->end();
        m_fbo->release();
    }
}

QMatRead::QMatRead(QQuickItem *parent)
    : QQuickItem(parent)
    , m_input(0)
    , m_font()
    , m_color("#eee")
    , m_numberWidth(3)
    , m_squareCell(false)
{
    setFlag(ItemHasContents, true);
    m_font.setFamily("Courier New");
    m_font.setPixelSize(12);
}

QMatRead::~QMatRead(){
}

QSGNode *QMatRead::updatePaintNode(QSGNode *oldNode, QQuickItem::UpdatePaintNodeData *){
    QMatReadNode* node = static_cast<QMatReadNode*>(oldNode);
    if (!node)
        node = new QMatReadNode(window());

    node->setRect(boundingRect());
    node->render(m_input, m_font, m_color, m_numberWidth, m_squareCell);

    return node;
}

void QMatRead::calculateImplicitSize(){
    if ( m_input ){
        Mat* renderSource = m_input->cvMat();
        if ( renderSource ){
            int cellHeight = ( m_font.pixelSize() + 2 ) * renderSource->channels() + 4;
            int cellWidth = 4 + m_numberWidth * ( m_font.pixelSize() / 3 * 2 );
            if ( m_squareCell ){
                cellHeight = cellWidth > cellHeight ? cellWidth : cellHeight;
                cellWidth  = cellHeight;
            }
            setImplicitSize(cellWidth * renderSource->cols, cellHeight * renderSource->rows);
        }
    }
}
