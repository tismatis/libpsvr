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
//Control Thread
//---------------------------------------------
class ControlThread : public QThread {
	Q_OBJECT

public:
	ControlThread(struct psvr_context *ctx);

public:
	void run() override;
	void stopThread();
	
signals:
	void frameUpdate(void *frame);

private:
	struct psvr_context * ctx;
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
	void controlFrame(void *frame);

	void headsetOn();
	void headsetOff();
	void enableVRMode();
	void enableVRMode2();
	void getDeviceInfo();
	void turnProcessorOff();
	void clearProcesorBuffer();

	void sendManualCommand();

private:
	struct psvr_context *ctx;

	Ui_DataViewerClass ui;

	SensorThread *sensorThread;
	ControlThread *controlThread;

private:
	static void psvr_logger(const char* msg, va_list args);

	static QTextEdit* loggingArea;
};
//---------------------------------------------
