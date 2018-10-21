import datetime
import serial
import firebase_admin
from firebase_admin import credentials
from firebase_admin import firestore

cred = credentials.Certificate("serviceAccountKey.json")
firebase_admin.initialize_app(cred, {
      'projectId': 'temperaturelogger-e35b4',
      })

db = firestore.client()
ser = serial.Serial('/dev/tty.usbmodem144101', 9600)

doc_ref = db.collection(u'temperatures')

while True:
      response = str(ser.readline())

      time = datetime.datetime.now().strftime('%H:%M')
      ser.write(time)    

      splitResponse = response.split("-")
      if len(splitResponse) == 2:
            doc_ref.add({
            u'temperature': float(splitResponse[0]),
            u'humidity': float(splitResponse[1]),
            u'date': datetime.datetime.now()
            })