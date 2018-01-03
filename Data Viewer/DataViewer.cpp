#include "DataViewer.h"
#include "psvr.h"

//---------------------------------------------
//Sensor Thread
//---------------------------------------------
SensorThread::SensorThread(struct psvr_context *ctx) {
	this->ctx = ctx;
	this->running = true;
	this->updateSpd = 0;
}

void SensorThread::run() {
	if (!ctx) return; //dont have a context, dont run.
	union psvr_sensor_frame frame;

	while (running) {
		psvr_read_sensor_sync(ctx, (uint8_t *)&frame, sizeof(union psvr_sensor_frame));
		emit frameUpdate(&frame);

		switch (this->updateSpd) {
		case 1:
			msleep(500);
			break;
		case 2:
			msleep(1000);
			break;
		case 3:
			msleep(2000);
			break;
		case 4:
			msleep(3000);
			break;
		}
	}
}

void SensorThread::setUpdateSpeed(int spd) { this->updateSpd = spd; }
void SensorThread::stopThread() { this->running = false; }
//---------------------------------------------

//---------------------------------------------
//Data Viewer Window
//---------------------------------------------
DataViewer::DataViewer(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);

	psvr_open(&(this->ctx)); //open psvr context

	this->sensorThread = new SensorThread(this->ctx);
	connect(
		ui.cmbx_update_spd,
		SIGNAL(
			currentIndexChanged(int)
		),
		this->sensorThread,
		SLOT(
			setUpdateSpeed(int)
		)
	);
	connect(
		this->sensorThread,
		SIGNAL(
			frameUpdate(void*)
		),
		this,
		SLOT(
			sensorFrame(void*)
		)
	);
}

DataViewer::~DataViewer() {
	this->sensorThread->stopThread();
	this->sensorThread->wait();

	psvr_close(this->ctx); //close the context
}

void DataViewer::sensorFrame(void *data) {
	union psvr_sensor_frame* frame = (union psvr_sensor_frame*)data;
}
//---------------------------------------------
