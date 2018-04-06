from naoqi import ALProxy

tts = ALProxy("ALTextToSpeech", "192.168.10.102", 9559)
tts.say("Hello world")
