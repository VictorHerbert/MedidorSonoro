from gtts import gTTS
import os 
  
class DFPlayerMiniAudioGen:
    
    def __init__(self, language = 'pt-br'):
        self.language = language
        self.audios = []
    
    def putTexts(self,texts):
        self.audios.extend(texts)
            
    def putNumbers(self,nums):
        self.audios.extend(map(lambda x: str(x), nums))
            
    def generateFiles(self,path):
        for i in range(len(self.audios)):
            myobj = gTTS(text=self.audios[i], lang=self.language, slow=False) 
            filename = path +  str(i).zfill(4) + '.mp3'
            myobj.save(filename) 
            print(filename + '|' + self.audios[i])


falas = ['e','ponto','conduz','não conduz','Medição de PH','Medição de temperatura','Identificador de soluções eletrolíticas','O ph é','A temperatura é','Graus Celsius','Dispositivo pronto para uso']
path = 'C:\\Users\\victo\\Desktop\\Files\\'

audioGen = DFPlayerMiniAudioGen()
audioGen.putNumbers(list(range(0,200)))
audioGen.putTexts(falas)
audioGen.generateFiles(path)


