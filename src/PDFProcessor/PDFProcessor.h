//
// Created by justin on 2020/11/18.
//
#pragma once

#include <QWidget>
#include <QUrl>

#include <QLoggingCategory>
#include <QMainWindow>
#include <QtPdfWidgets/QPdfView>

Q_DECLARE_LOGGING_CATEGORY(lcExample)

class QPdfDocument;
class QPdfView;

class PageSelector;
class ZoomSelector;

class PDFProcessor : public QWidget
{
Q_OBJECT

public:
    explicit PDFProcessor(QWidget *parent = nullptr);
    ~PDFProcessor();

public slots:
    void open(const QUrl &docLocation);

private slots:
    void bookmarkSelected(const QModelIndex &index);

    // action handlers
    void on_actionOpen_triggered();
    void on_actionQuit_triggered();
    void on_actionAbout_triggered();
    void on_actionAbout_Qt_triggered();
    void on_actionZoom_In_triggered();
    void on_actionZoom_Out_triggered();
    void on_actionPrevious_Page_triggered();
    void on_actionNext_Page_triggered();
//    void on_actionContinuous_triggered();

private:

    ZoomSelector *m_zoomSelector;
    PageSelector *m_pageSelector;

    QPdfDocument *m_document;
    QPdfView* m_pdfView;
};
