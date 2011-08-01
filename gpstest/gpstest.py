import serial
import sys
import threading
from optparse import OptionParser

class TrackuinoTest:
  def __init__(self):
    self.end = False

  def gpsTest(self, name, port, bauds):
    self.s = serial.Serial(port, bauds)
    file = open(name)
    r = threading.Thread(target=self.reader)
    r.start()

    for line in file:
      if self.end:
        break
      self.s.write(line)

    self.end = True
    r.join()
    self.s.close()

  def reader(self):
    while not self.end:
      try:
        data = self.s.read(1)
        sys.stdout.write(data)
        data = self.s.read(self.s.inWaiting())
        sys.stdout.write(data)
        sys.stdout.flush()
      except:
        print "Exception. Aborting..."
        raise

  def stop(self):
    self.end = True

if __name__ == "__main__":

  parser = OptionParser()
  parser.add_option("-p", "--port", dest="port", type="string", 
      help="Serial port")
  parser.add_option("-b", "--bauds", dest="bauds", type="int",
      help="Baud rate", default=9600)
  parser.add_option("-f", "--log", dest="logfile", type="string",
      help="Log file to replay")

  (options, args) = parser.parse_args()

  if options.port is None:
    parser.error("Option -p is mandatory")

  if options.logfile is None:
    parser.error("Option -f is mandatory")

  try:
    t = TrackuinoTest()
    t.gpsTest(options.logfile, options.port, options.bauds)
  except KeyboardInterrupt:
    t.stop()

