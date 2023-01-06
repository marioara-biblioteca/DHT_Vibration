import serial
from PyQt5 import QtWidgets, QtCore
from PyQt5.QtWidgets import QPushButton
import pyqtgraph as pg
import sys


ser_com = serial.Serial('COM8', 9600)


class MainWindow(QtWidgets.QMainWindow):

    def __init__(self, *args, **kwargs):
        super(MainWindow, self).__init__(*args, **kwargs)
        self.setWindowTitle("Microcontroller Window")

        self.graphWidget = pg.PlotWidget()
        self.setCentralWidget(self.graphWidget)
        self.graphWidget.setBackground((245, 245, 245))
        self.graphWidget.setTitle("\nVibration Data\n", color="black", size="25px")

        styles = {'color': 'black', 'font-size': '15px'}
        self.graphWidget.setLabel('left', 'Vibration', **styles)
        self.graphWidget.setLabel('bottom', 'Time', **styles)
        self.graphWidget.showGrid(x=True, y=True)
        self.graphWidget.setYRange(0, 6000, padding=0)

        self.x = list(range(50))
        self.y = list(range(50))

        pen = pg.mkPen(color=(0, 128, 128), width=5)
        self.data_line = self.graphWidget.plot(self.x, self.y, pen=pen)

        self.timer = QtCore.QTimer()
        self.timer.setInterval(1)
        self.timer.timeout.connect(self.update_plot_data)
        self.timer.start()
        self.initUI()

    def initUI(self):

        self.setGeometry(100, 100, 1600, 900)
        self.redButton = QPushButton(self)
        self.redButton.setText("Red")
        self.redButton.clicked.connect(self.signalRed)
        self.redButton.move(0, 0)

        self.greenButton = QPushButton(self)
        self.greenButton.setText("Green")
        self.greenButton.clicked.connect(self.signalGreen)
        self.greenButton.move(100, 0)

        self.blueButton = QPushButton(self)
        self.blueButton.setText("Blue")
        self.blueButton.clicked.connect(self.signalBlue)
        self.redButton.move(200, 0)

        self.overrideButton = QPushButton(self)
        self.overrideButton.setText("OFF")
        self.overrideButton.clicked.connect(self.signalOff)
        self.overrideButton.move(300, 0)

    def update_plot_data(self):
        data = ser_com.readline().decode('utf-8')
        try:
            for line in data.split('\n'):
                line = line.strip('\r')
                if not line[0] == 'V':
                    continue
                val = line[1:]
                if val == '':
                    floatdata = 0
                else:
                    floatdata = float(val)

                self.x = self.x[1:]
                self.x.append(self.x[-1] + 1)

                self.y = self.y[1:]

                self.y.append(floatdata)
                self.data_line.setData(self.x, self.y)
        except:
            return

    def signalRed(self):
        ser_com.write(b'1')

    def signalGreen(self):
        ser_com.write(b'2')

    def signalBlue(self):
        ser_com.write(b'3')

    def signalOff(self):
        ser_com.write(b'0')


def main():
    ser_com.write(b'0')
    while True:
        data = ser_com.readline()
        data = data.decode("utf-8")

        # print(data)


if __name__ == '__main__':
    # main()
    app = QtWidgets.QApplication(sys.argv)

    SensorWindow = MainWindow()
    SensorWindow.show()
    sys.exit(app.exec_())
