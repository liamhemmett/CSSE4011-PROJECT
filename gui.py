import io
import paho.mqtt.client as mqtt
from PIL import Image, ImageTk
import tkinter as tk

# MQTT Configuration
MQTT_BROKER = 'your_mqtt_broker_address'
MQTT_PORT = 1883
MQTT_TOPIC = 'image/topic'

# Tkinter setup for displaying the image
root = tk.Tk()
root.title("MQTT Image Viewer")
image_label = tk.Label(root)
image_label.pack()

def on_connect(client, userdata, flags, rc):
    print("Connected with result code " + str(rc))
    client.subscribe(MQTT_TOPIC)

def on_message(client, userdata, msg):
    if msg.topic == MQTT_TOPIC:
        # Load the image from the message payload
        image_stream = io.BytesIO(msg.payload)
        image = Image.open(image_stream)
        
        # Update the displayed image
        tk_image = ImageTk.PhotoImage(image)
        image_label.config(image=tk_image)
        image_label.image = tk_image  # keep a reference!
        root.update()

client = mqtt.Client()
client.on_connect = on_connect
client.on_message = on_message

client.connect(MQTT_BROKER, MQTT_PORT, 60)

# Start the loop
client.loop_start()

# Start the Tkinter GUI loop
root.mainloop()
