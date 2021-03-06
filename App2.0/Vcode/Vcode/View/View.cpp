#include <View\View.h>
#include <QFileDialog>
#include <QMessageBox>
#include <qdebug.h>


View::View(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	ui.importPicAction->setShortcuts(QKeySequence::Open);
	ui.importPicAction->setStatusTip(tr("Import a picture of verification code"));

	originScene = new QGraphicsScene();
	grayScene = new QGraphicsScene();
	denoiseScene = new QGraphicsScene();
	removeBGScene = new QGraphicsScene();
	binaryScene = new QGraphicsScene();

	ui.leftOriginView->setScene(originScene);
	ui.rightOriginView->setScene(originScene);
	/*ui.binaryzationView->setScene(binaryScene);
	ui.denoiseView->setScene(denoiseScene);
	ui.removeBgView->setScene(removeBGScene);*/
	ui.binaryzationView->setScene(grayScene);
	ui.denoiseView->setScene(grayScene);
	ui.removeBgView->setScene(grayScene);
	ui.grayView->setScene(grayScene);
	connect(ui.importPicAction, &QAction::triggered, this, &View::importPicture);
	connect(ui.confirmButton, &QPushButton::clicked, this, &View::processPicture);
	//connect(ui.recognizeButton, SIGNAL(clicked()), this, SLOT(recognizeCode()));
}
void View::processPicture() {
	/*shared_ptr<StringParam> sp = make_shared<StringParam>();
	sp->setPath(filename.toStdString());*/
	if (ui.aveButton->isChecked()) {
		grayType = GrayType::GRAY_AVERAGE;
	}
	else if (ui.maxbutton->isChecked()) {
		grayType = GrayType::GRAY_MAX;
	}
	else if (ui.weightAveButton->isChecked()) {
		grayType = GrayType::GRAY_WEIGHTAVE;
	}
	else {
		grayType = -1;
	}
	shared_ptr<GrayTypeParam> sp = make_shared<GrayTypeParam>();
	sp->setType(grayType);
	processPictureCommand->setParams(static_pointer_cast<Param, GrayTypeParam>(sp));

	processPictureCommand->exec();
}
void View::importPicture() {

	filename = QFileDialog::getOpenFileName(this, "Select Picture", ".", tr("Images (*.png *.bmp *.jpg *.tif *.GIF )"));

	if (!filename.isEmpty()) {

		shared_ptr<StringParam> sp = make_shared<StringParam>();
		sp->setPath(filename.toStdString());

		loadPictureCommand->setParams(static_pointer_cast<Param, StringParam>(sp));

		loadPictureCommand->exec();
	}
}
void View::update(const string& attribute) {
	if (attribute == "image") {
		originScene->clear();
		originScene->addPixmap(QPixmap::fromImage(pImg->scaled(ui.leftOriginView->width(), ui.leftOriginView->height(), Qt::KeepAspectRatio)));
	}
	else if (attribute == "process") {
		grayScene->clear();
		grayScene->addPixmap(QPixmap::fromImage(pGrayImg->scaled(ui.grayView->width(), ui.grayView->height(), Qt::KeepAspectRatio)));
		denoiseScene->clear();
		denoiseScene->addPixmap(QPixmap::fromImage(pDenoiseImg->scaled(ui.denoiseView->width(), ui.denoiseView->height(), Qt::KeepAspectRatio)));
		removeBGScene->clear();
		removeBGScene->addPixmap(QPixmap::fromImage(pRemoveBGImg->scaled(ui.removeBgView->width(), ui.removeBgView->height(), Qt::KeepAspectRatio)));
		binaryScene->clear();
		binaryScene->addPixmap(QPixmap::fromImage(pBinaryImg->scaled(ui.binaryzationView->width(), ui.binaryzationView->height(), Qt::KeepAspectRatio)));
	}
}
void View::commandSucceed(bool flag) {
	if (!flag) {
		QMessageBox::critical(this, "Error", "Error!");
	}
}