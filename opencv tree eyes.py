import cv2
import time
import serial
import os

cascade_path = 'haarcascade_frontalface_default.xml'
000
if not os.path.isfile(cascade_path):
    raise RuntimeError(f"Файл {cascade_path} не найден.")

face_cascade = cv2.CascadeClassifier(cascade_path)


if face_cascade.empty():
    raise RuntimeError(f"Хаар забыл, гений {cascade_path}")


cap = cv2.VideoCapture(0)

if not cap.isOpened():
    print("Error: Could not open webcam.")
    exit()


ser = serial.Serial('COM8', 9600)#COM-порт

while True:
    ret, frame = cap.read()
    
    if not ret:
        print("Error: Could not read frame.")
        break

    gray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)
    
    gray = cv2.GaussianBlur(gray, (5, 5), 0)
    
    faces = face_cascade.detectMultiScale(gray, scaleFactor=1.1, minNeighbors=8, minSize=(50, 50))
    
    if len(faces) > 0:
        largest_face = max(faces, key=lambda rect: rect[2] * rect[3])
        (x, y, w, h) = largest_face
        
        cv2.rectangle(frame, (x, y), (x+w, y+h), (255, 0, 255), 2)
        
        cv2.putText(frame, f'X: {x}, Y: {y}', (x, y - 10), cv2.FONT_HERSHEY_SIMPLEX, 0.5, (255, 0, 0), 2)
        
        if 0 <= x <= 184:
            direction = 'l'
            print('left')
        elif 184 < x <= 368:
            direction = 'c'
            print('center')
        elif 368 < x <= 500:
            direction = 'r'
            print('right')
        else:
            direction = 'c'
        
        ser.write(direction.encode())
        time.sleep(1)
    
    cv2.imshow('Video', frame)
    
    if cv2.waitKey(1) & 0xFF == ord('q'):
        break

cap.release()
ser.close()
cv2.destroyAllWindows()
