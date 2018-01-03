#pragma once

#include <QtWidgets/QMainWindow>

#include <qthread.h>

#include "ui_DataViewer.h"

//---------------------------------------------
//Sensor Thread
//---------------------------------------------
class SensorThread : public QThread {
	Q_OBJECT

public:
	SensorThread(struct psvr_context *ctx);

public:
	void run() override;
	void stopThread();

public slots:
	void setUpdateSpeed(int spd);

signals:
	void frameUpdate(void *frame);

private:
	struct psvr_context * ctx;
	bool running;
	int updateSpd;
};
//---------------------------------------------

//---------------------------------------------
//Data Viewer Window
//---------------------------------------------
class DataViewer : public QMainWindow
{
	Q_OBJECT

public:
	DataViewer(QWidget *parent = Q_NULLPTR);
	~DataViewer();

public slots:
	void sensorFrame(void *frame);

private:
	struct psvr_context *ctx;

	Ui_DataViewerClass ui;

	SensorThread* sensorThread;
};
//---------------------------------------------
