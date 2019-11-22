from gtts import gTTS
import os 
  
class DFPlayerMiniAudioGen:
    
    def __init__(self, language = 'pt-br', audioIndex = 0):
        self.language = language
        self.audios = []
        self.audioIndex = audioIndex
    
    def putTexts(self,texts):
        self.audios.extend(texts)
            
    def putNumbers(self,nums):
        self.audios.extend(map(lambda x: str(x), nums))
            
    def generateFiles(self,path):
        for i in range(len(self.audios)):
            audioTTS = gTTS(text=self.audios[i], lang=self.language, slow=False) 
            filename = path +  str(i+self.audioIndex).zfill(4) + '.mp3'
            audioTTS.save(filename) 
            print(filename + '|' + self.audios[i])


falas = ['e','ponto','conduz','não conduz','Medição de PH','Medição de temperatura','Identificador de soluções eletrolíticas','O ph é','A temperatura é','Graus Celsius','Dispositivo pronto para uso']
path = 'Files\\'


audioGen = DFPlayerMiniAudioGen()
audioGen.putNumbers(list(range(0,200)))
audioGen.putTexts(falas)
audioGen.generateFiles(path)


