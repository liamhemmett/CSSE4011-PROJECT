import sys
import io
import paho.mqtt.client as mqtt
from PIL import Image
import numpy as np
from PyQt5.QtWidgets import QApplication, QLabel, QMainWindow
from PyQt5.QtGui import QPixmap, QImage
from PyQt5.QtCore import Qt

# MQTT Configuration
MQTT_BROKER = 'csse4011-iot.zones.eait.uq.edu.au'
MQTT_PORT = 1883
MQTT_TOPIC = 's4697686'

class ImageWindow(QMainWindow):
    def __init__(self):
        super().__init__()
        self.setWindowTitle("MQTT Image Viewer")
        self.image_label = QLabel(self)
        self.setCentralWidget(self.image_label)
        self.resize(800, 600)

    def update_image(self, q_image):
        pixmap = QPixmap.fromImage(q_image)
        self.image_label.setPixmap(pixmap)
        self.image_label.setScaledContents(True)

def on_connect(client, userdata, flags, rc):
    print("Connected with result code " + str(rc))
    client.subscribe(MQTT_TOPIC)

def on_message(client, userdata, msg):
    print("GOT MESSAGE")
    if msg.topic == MQTT_TOPIC:
        try:
            # Calculate the height 'A' based on the length of the payload
            image_width = 255
            payload_length = len(msg.payload)
            image_height = payload_length // image_width

            if payload_length % image_width == 0:
                # Convert the byte data to a numpy array and reshape it
                image_data = np.frombuffer(msg.payload, dtype=np.uint8).reshape((image_height, image_width))

                # Convert the numpy array to QImage
                q_image = QImage(image_data.data, image_data.shape[1], image_data.shape[0], QImage.Format_Grayscale8)

                # Resize the QImage for better display
                q_image = q_image.scaled(800, 600, Qt.KeepAspectRatio)

                # Update the image in the window
                window.update_image(q_image)
            else:
                print(f"Received data length is not a multiple of {image_width}: {payload_length}")
        except Exception as e:
            print(f"Failed to process image: {e}")

app = QApplication(sys.argv)
window = ImageWindow()
window.show()

client = mqtt.Client()
client.on_connect = on_connect
client.on_message = on_message

try:
    client.connect(MQTT_BROKER, MQTT_PORT, 60)
except Exception as e:
    print(f"Failed to connect to MQTT broker: {e}")
    sys.exit(1)

# Start the loop
client.loop_start()

# Start the PyQt event loop
sys.exit(app.exec_())
