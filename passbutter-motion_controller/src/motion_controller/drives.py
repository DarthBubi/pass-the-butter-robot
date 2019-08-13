import ThunderBorg
import time
import sys

class Drives:
    TB = ThunderBorg.ThunderBorg()
    motorControlEnabled = False

    def __init__(self):
        self.TB.Init()
        if not self.TB.foundChip:
            boards = ThunderBorg.ScanForThunderBorg()
            if len(boards) == 0:
                print 'no ThunderBorg found. check you are attached..'
            else:
                print 'no ThunderBorg at address %02X. but we did find boards:' % (TB.i2cAddress)
                for board in boards:
                    print '  %02X (%d)' % (board, board)
                print 'if you need to change the I2C address change the setup line so it is correct, e.g.'
                print 'TB.i2cAddress = 0x%02X' % (boards[0])
            raise Exception('init motor control failed')

        self.enable()

    def checkEnabled(self):
        if not self.motorControlEnabled:
            print "motor control is disabled. call enable() to activate the motor control."

        return self.motorControlEnabled

    def setMotor1(self, val):
        if not self.checkEnabled():
            return

        print "set motor 1 to %f" % (val)
        self.TB.SetMotor1(val)

    def setMotor2(self, val):
        if not self.checkEnabled():
            return

        print "set motor 2 to %f" % (val)
        self.TB.SetMotor2(val)

    def disable(self):
        if not self.motorControlEnabled:
            print "motor control allready disabled.."
            return

        print "disable motors control.."
        self.motorControlEnabled = False
        self.TB.MotorsOff()

    def enable(self):
        if self.motorControlEnabled:
            print "motor control allready enabled.."
            return

        print "enable motor control. ready for motor commands."
        self.motorControlEnabled = True
