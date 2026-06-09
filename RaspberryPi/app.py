from flask import Flask, Response
from picamera2 import Picamera2
import cv2
import serial
import threading

app = Flask(**name**)

distance = "Unknown"
status = "TRACK_CLEAR"
gate = "OPEN"
train_count = 0

arduino = serial.Serial('/dev/ttyUSB0', 9600, timeout=1)

picam2 = Picamera2()
picam2.configure(
picam2.create_preview_configuration(
main={"size": (640, 480)}
)
)
picam2.start()

def serial_reader():
global distance, status, gate, train_count

```
while True:
    try:
        line = arduino.readline().decode('utf-8').strip()

        if line.startswith("DIST="):
            distance = line.split("=")[1]

        elif line.startswith("STATUS="):

            status = line.split("=")[1]

            if status == "GATE_CLOSED":
                gate = "CLOSED"

            elif status == "TRACK_CLEAR":
                gate = "OPEN"

        elif line.startswith("COUNT="):
            train_count = int(line.split("=")[1])

    except:
        pass
```

threading.Thread(
target=serial_reader,
daemon=True
).start()

def generate_frames():

```
while True:

    frame = picam2.capture_array()

    ret, buffer = cv2.imencode('.jpg', frame)

    if not ret:
        continue

    yield (
        b'--frame\r\n'
        b'Content-Type: image/jpeg\r\n\r\n'
        + buffer.tobytes()
        + b'\r\n'
    )
```

@app.route('/')
def home():

```
status_color = "green"

if status == "TRAIN_DETECTED":
    status_color = "red"

elif status == "GATE_CLOSED":
    status_color = "red"

return f"""
<html>

<head>
    <meta http-equiv="refresh" content="1">
    <title>Smart Railway Crossing</title>
</head>

<body>

    <h1>SMART RAILWAY CROSSING</h1>

    <h2>Distance: {distance} cm</h2>

    <h2 style="color:{status_color};">
        Status: {status}
    </h2>

    <h2>Gate: {gate}</h2>

    <h2>Train Count: {train_count}</h2>

    <img src="/video_feed" width="800">

</body>

</html>
"""
```

@app.route('/video_feed')
def video_feed():

```
return Response(
    generate_frames(),
    mimetype='multipart/x-mixed-replace; boundary=frame'
)
```

if **name** == '**main**':
app.run(
host='0.0.0.0',
port=5000,
debug=False
)
