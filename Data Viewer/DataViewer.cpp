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
	psvr_sensor_frame frame;

	while (running) {
		psvr_read_sensor_sync(ctx, (uint8_t *)&frame, sizeof(psvr_sensor_frame));
		emit frameUpdate(&frame);

		switch (this->updateSpd) {
		default:
			yieldCurrentThread();
			break;
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

	psvr_context *ctx;

	int err = psvr_open(&ctx); //open psvr context

	this->ctx = ctx;
	
	this->sensorThread = nullptr;

	if (err == 0) {
		this->sensorThread = new SensorThread(this->ctx);
		connect(
			this->ui.cmbx_update_spd,
			SIGNAL(currentIndexChanged(int)),
			this->sensorThread,
			SLOT(setUpdateSpeed(int))
		);
		connect(
			this->sensorThread,
			SIGNAL(frameUpdate(void*)),
			this,
			SLOT(sensorFrame(void*)),
			Qt::BlockingQueuedConnection
		);

		this->sensorThread->start();
	}
}

DataViewer::~DataViewer() {
	if (this->sensorThread) {
		this->sensorThread->stopThread();
		this->sensorThread->wait();

		disconnect(
			this->ui.cmbx_update_spd,
			SIGNAL(currentIndexChanged(int)),
			this->sensorThread,
			SLOT(setUpdateSpeed(int))
		);
		disconnect(
			this->sensorThread,
			SIGNAL(frameUpdate(void*)),
			this,
			SLOT(sensorFrame(void*))
		);

		delete this->sensorThread;
		this->sensorThread = nullptr;
	}

	psvr_close(this->ctx); //close the context
}

void DataViewer::sensorFrame(void *data) {
	psvr_sensor_frame* frame = (psvr_sensor_frame*)data;

	ui.lbl_button_reserved->setText(frame->s.button.reserved ? "True" : "False");
	ui.lbl_btn_plus->setText(frame->s.button.plus ? "True" : "False");
	ui.lbl_btn_minus->setText(frame->s.button.minus ? "True" : "False");
	ui.lbl_btn_mute->setText(frame->s.button.mute ? "True" : "False");

	ui.lbl_reserved0->setText(QString::number(frame->s.reserved0));
	ui.lbl_volume->setText(QString::number(frame->s.volume));

	QString reserved1;
	for (int i = 0; i < 5; i++) {
		reserved1.append(QString::number(frame->s.reserved1[i]));
		reserved1.append(" ");
	}
	ui.lbl_reserved1->setText(reserved1);

	ui.lbl_status_worn->setText(frame->s.status.worn ? "True" : "False");
	ui.lbl_status_disp_active->setText(frame->s.status.display_active ? "True" : "False");
	ui.lbl_status_hdmi_dc->setText(frame->s.status.hdmi_disconnected ? "True" : "False");
	ui.lbl_status_mic_mute->setText(frame->s.status.microphone_muted ? "True" : "False");
	ui.lbl_status_headphone_connect->setText(frame->s.status.headphone_connected ? "True" : "False");
	ui.lbl_status_reserved->setText(frame->s.status.reserved ? "True" : "False");
	ui.lbl_status_tick->setText(frame->s.status.tick ? "True" : "False");

	QString reserved2;
	for (int i = 0; i < 11; i++) {
		reserved2.append(QString::number(frame->s.reserved2[i]));
		reserved2.append(" ");
	}
	ui.lbl_reserved2->setText(reserved2);

	ui.lbl_data1_gyro_yaw->setText(QString::number(frame->s.data[0].gyro.yaw));
	ui.lbl_data1_gyro_pitch->setText(QString::number(frame->s.data[0].gyro.pitch));
	ui.lbl_data1_gyro_roll->setText(QString::number(frame->s.data[0].gyro.roll));
	ui.lbl_data1_accel_x->setText(QString::number(frame->s.data[0].accel.x));
	ui.lbl_data1_accel_y->setText(QString::number(frame->s.data[0].accel.y));
	ui.lbl_data1_accel_z->setText(QString::number(frame->s.data[0].accel.z));

	ui.lbl_data2_gyro_yaw->setText(QString::number(frame->s.data[1].gyro.yaw));
	ui.lbl_data2_gyro_pitch->setText(QString::number(frame->s.data[1].gyro.pitch));
	ui.lbl_data2_gyro_roll->setText(QString::number(frame->s.data[1].gyro.roll));
	ui.lbl_data2_accel_x->setText(QString::number(frame->s.data[1].accel.x));
	ui.lbl_data2_accel_y->setText(QString::number(frame->s.data[1].accel.y));
	ui.lbl_data2_accel_z->setText(QString::number(frame->s.data[1].accel.z));

	QString reserved3;
	for (int i = 0; i < 12; i++) {
		reserved3.append(QString::number(frame->s.reserved3[i]));
		reserved3.append(" ");
	}
	ui.lbl_reserved3->setText(reserved3);

	QString raw;
	for (int i = 0; i < 64; i++) {
		raw.append(QString::number(frame->raw[i]));
		raw.append(" ");
		if (i % 12 == 0) raw.append("<br />");
	}
	ui.lbl_sensor_raw->setText(raw);
}
//---------------------------------------------
