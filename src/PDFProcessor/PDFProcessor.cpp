//
// Created by justin on 2020/11/18.
//
#include "PDFProcessor.h"

#include "PageSelector.h"
#include "ZoomSelector.h"

#include <QFileDialog>
#include <QMessageBox>
#include <QtPdf/QPdfBookmarkModel>
#include <QtPdf/QPdfDocument>
#include <QtPdf/QPdfPageNavigation>
#include <QtMath>
#include <QToolBar>
#include <QTreeView>

const qreal zoomMultiplier = qSqrt(2.0);

Q_LOGGING_CATEGORY(lcExample, "qt.examples.pdfviewer")

PDFProcessor::PDFProcessor(QWidget *parent) : QWidget(parent), m_zoomSelector(new ZoomSelector(this)),
                                              m_pageSelector(new PageSelector(this)),
                                              m_document(new QPdfDocument(this)) {
    m_zoomSelector->setMaximumWidth(150);
    QToolBar* mainToolBar = new QToolBar(this);
    QAction* actionZoom_In = new QAction(this);
    mainToolBar->insertWidget(actionZoom_In, m_zoomSelector);

    m_pageSelector->setMaximumWidth(150);
    mainToolBar->addWidget(m_pageSelector);

    m_pdfView = new QPdfView(this);

    m_pageSelector->setPageNavigation(m_pdfView->pageNavigation());

    connect(m_zoomSelector, &ZoomSelector::zoomModeChanged, m_pdfView, &QPdfView::setZoomMode);
    connect(m_zoomSelector, &ZoomSelector::zoomFactorChanged, m_pdfView, &QPdfView::setZoomFactor);
    m_zoomSelector->reset();

    QPdfBookmarkModel *bookmarkModel = new QPdfBookmarkModel(this);
    bookmarkModel->setDocument(m_document);

    QTreeView* bookmarkView = new QTreeView(this);
    bookmarkView->setModel(bookmarkModel);
    connect(bookmarkView, SIGNAL(activated(QModelIndex)), this, SLOT(bookmarkSelected(QModelIndex)));

    QTabWidget* tabWidget = new QTabWidget(this);
    tabWidget->setTabEnabled(1, false); // disable 'Pages' tab for now

    m_pdfView->setDocument(m_document);

    connect(m_pdfView, &QPdfView::zoomFactorChanged, m_zoomSelector, &ZoomSelector::setZoomFactor);
}

PDFProcessor::~PDFProcessor() {
}

void PDFProcessor::open(const QUrl &docLocation) {
    if (docLocation.isLocalFile()) {
        m_document->load(docLocation.toLocalFile());
        const auto documentTitle = m_document->metaData(QPdfDocument::Title).toString();
        setWindowTitle(!documentTitle.isEmpty() ? documentTitle : QStringLiteral("PDF Viewer"));
    } else {
        qCDebug(lcExample) << docLocation << "is not a valid local file";
        QMessageBox::critical(this, tr("Failed to open"),
                              tr("%1 is not a valid local file").arg(docLocation.toString()));
    }
    qCDebug(lcExample) << docLocation;
}

void PDFProcessor::bookmarkSelected(const QModelIndex &index) {
    if (!index.isValid())
        return;

    const int page = index.data(QPdfBookmarkModel::PageNumberRole).toInt();
    m_pdfView->pageNavigation()->setCurrentPage(page);
}

void PDFProcessor::on_actionOpen_triggered() {
    QUrl toOpen = QFileDialog::getOpenFileUrl(this, tr("Choose a PDF"), QUrl(), "Portable Documents (*.pdf)");
    if (toOpen.isValid())
        open(toOpen);
}

void PDFProcessor::on_actionQuit_triggered() {
//    QApplication::quit();
}

void PDFProcessor::on_actionAbout_triggered() {
    QMessageBox::about(this, tr("About PdfViewer"),
                       tr("An example using QPdfDocument"));
}

void PDFProcessor::on_actionAbout_Qt_triggered() {
    QMessageBox::aboutQt(this);
}

void PDFProcessor::on_actionZoom_In_triggered() {
    m_pdfView->setZoomFactor(m_pdfView->zoomFactor() * zoomMultiplier);
}

void PDFProcessor::on_actionZoom_Out_triggered() {
    m_pdfView->setZoomFactor(m_pdfView->zoomFactor() / zoomMultiplier);
}

void PDFProcessor::on_actionPrevious_Page_triggered() {
    m_pdfView->pageNavigation()->goToPreviousPage();
}

void PDFProcessor::on_actionNext_Page_triggered() {
    m_pdfView->pageNavigation()->goToNextPage();
}

//void PDFProcessor::on_actionContinuous_triggered() {
//    m_pdfView->setPageMode(ui->actionContinuous->isChecked() ? QPdfView::MultiPage : QPdfView::SinglePage);
//}