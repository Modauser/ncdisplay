from PIL import Image
import gameduino2
import gameduino2.prep
import gameduino2.registers

class Spritesheet(gameduino2.prep.AssetBin):
    def __init__(self):
        gameduino2.prep.AssetBin.__init__(self)
        self.header = 'assets.h'
        self.asset_file = 'assets.gd3'
        self.target_810()

    def addall(self):
        sheet = gameduino2.prep.split(272, 480, Image.open("sleep1.png"))
        self.load_handle("SLEEP", sheet, gameduino2.registers.ARGB4)
        sheet = gameduino2.prep.split(112, 296, Image.open("ani1.png"))
        self.load_handle("ANI1", sheet, gameduino2.registers.ARGB4)
        sheet = gameduino2.prep.split(112, 296, Image.open("ani2.png"))
        self.load_handle("ANI2", sheet, gameduino2.registers.ARGB4)
        sheet = gameduino2.prep.split(112, 296, Image.open("ani3.png"))
        self.load_handle("ANI3", sheet, gameduino2.registers.ARGB4)
        sheet = gameduino2.prep.split(272, 198, Image.open("HomeWtr.png"))
        self.load_handle("HOMEWTR", sheet, gameduino2.registers.ARGB4)
        sheet = gameduino2.prep.split(50, 26, Image.open("IonLogo.png"))
        self.load_handle("IONLOGO", sheet, gameduino2.registers.ARGB4)
        sheet = gameduino2.prep.split(96, 92, Image.open("restart.png"))
        self.load_handle("RESTART", sheet, gameduino2.registers.ARGB2)

        sheet = gameduino2.prep.split(68, 67, Image.open("fltstrip.png"))
        self.load_handle("FLTSTRIP", sheet, gameduino2.registers.ARGB4)
        sheet = gameduino2.prep.split(60, 75, Image.open("dropstrp.png"))
        self.load_handle("DROPSTRP", sheet, gameduino2.registers.ARGB2)
        sheet = gameduino2.prep.split(26, 26, Image.open("setstrip.png"))
        self.load_handle("SETSTRIP", sheet, gameduino2.registers.ARGB4)

if __name__ == '__main__':
    Spritesheet().make()

