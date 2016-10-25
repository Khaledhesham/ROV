import socket
import math
TCP_IP = '127.0.0.1'
TCP_PORT = 9000
BUFFER_SIZE = 20  # Normally 1024, but we want fast response
axis = {'x': 0, 'y': 0, 'z': 0}
state = "rov-drive"
manipulator_state = "grip"
pwm_list = [1440,1440,1440,1440,1440,1440,1500];

s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.bind((TCP_IP, TCP_PORT))
s.listen(1)

def CalculateHorizontalMotors():
   resultant = math.hypot(axis['x'], axis['y'])
   theta = math.atan2(axis['y'], axis['x'])
   alfa = 0.785398 - theta
   RightComponent = resultant * math.cos(alfa)
   LeftComponent = resultant * math.sin(alfa)
   pwm_list[0] = int(1440 + (RightComponent * 8.1))
   print "front right thruster: ", pwm_list[0]
   pwm_list[1] = int(1440 + (LeftComponent * 8.1)) 
   print "front left thruster: ", pwm_list[1]
   pwm_list[2] = int(1440 - (RightComponent * 8.1)) 
   print "back right thruster: ", pwm_list[2]
   pwm_list[3] = int(1440 - (LeftComponent * 8.1)) 
   print "back left thruster: ", pwm_list[3]
   return;

def CalculateVerticalMotors():
   pwm_list[4] = int(1440 + (axis['z'] * 8.1)) 
   print "Up front thruster: ", pwm_list[4]
   pwm_list[5] = int(1440 + (axis['z'] * 8.1)) 
   print "Up back thruster: ", pwm_list[5]
   return;

conn, addr = s.accept()
print 'Connection address:', addr
while 1:
  data = conn.recv(BUFFER_SIZE)
  if not data: break
  print "received data:", data
  #conn.send(data)  # echo
  # data processing
  split_data = data.split()
  if split_data[0] == "x":
    axis['x'] = int(split_data[1])
    if state == "rov-drive":
      CalculateHorizontalMotors()
    # else do calculations based on manipulator design
  elif split_data[0] == "y":
    axis['y'] = int(split_data[1])
    if state == "rov-drive":
      CalculateHorizontalMotors()
    # else do calculations based on manipulator design
  elif split_data[0] == "z":
    axis['z'] = int(split_data[1])
    CalculateVerticalMotors()
  elif split_data[0] == "1":
    if state == "rov-drive":
      state = "manipulator-drive"
    else:
      state = "rov-drive"
    print state
  elif split_data[0] == "2":
    if manipulator_state != "grip":
      manipulator_state = "grip"
      print manipulator_state
  elif split_data[0] == "3":
    if pwm_list[0] < 2000:
      pwm_list[6] +=50
    print "camera servo: ", pwm_list[6]
  elif split_data[0] == "4":
    if manipulator_state != "release":
      manipulator_state = "release"
      print manipulator_state
  elif split_data[0] == "5":
    if pwm_list[0] >1000:
      pwm_list[6] -=50
    print "camera servo: ", pwm_list[6]
conn.close()