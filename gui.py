import sys
import io
import paho.mqtt.client as mqtt
from PIL import Image
from PyQt5.QtWidgets import QApplication, QLabel, QMainWindow
from PyQt5.QtGui import QPixmap, QImage
from PyQt5.QtCore import Qt

# MQTT Configuration
MQTT_BROKER = 'mqtt.m5stack.com'
MQTT_PORT = 1883
MQTT_TOPIC = 's4697686/image'

class ImageWindow(QMainWindow):
    def __init__(self):
        super().__init__()
        self.setWindowTitle("MQTT Image Viewer")
        self.image_label = QLabel(self)
        self.setCentralWidget(self.image_label)
        self.resize(800, 600)

    def update_image(self, image):
        q_image = QPixmap.fromImage(image)
        self.image_label.setPixmap(q_image)
        self.image_label.setScaledContents(True)

def on_connect(client, userdata, flags, rc):
    print("Connected with result code " + str(rc))
    client.subscribe(MQTT_TOPIC)

def on_message(client, userdata, msg):
    if msg.topic == MQTT_TOPIC:
        # Assuming msg.payload contains the 8x8 grayscale image data
        if len(msg.payload) == 64:
            
            # Create a PIL Image from the 8x8 grayscale data
            image = Image.frombytes('L', (8, 8), msg.payload)
            print (image)
            # Convert the PIL image to QImage
            q_image = QImage(image.tobytes(), image.width, image.height, QImage.Format_Grayscale8)
            # Resize the QImage for better display
            q_image = q_image.scaled(400, 400, Qt.KeepAspectRatio)

            window.update_image(q_image)

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
