import os
import vosk
import pyaudio
import ollama
import pyttsx3


# Загрузите модель Vosk (замените путь на путь к вашей модели)



# Initialize the speech recognizer
model = vosk.Model('vosk-model-en-us-0.22')
recognizer = vosk.KaldiRecognizer(model, 16000)

# Инициализируем микрофон для записи речи
p = pyaudio.PyAudio()
stream = p.open(format=pyaudio.paInt16, channels=1, rate=16000, input=True, frames_per_buffer=4096)
stream.start_stream()

def initialize_chat(model_name="llama2"):
    # Initialize history
    messages = []

    # Initialize model
    model = model_name

    return messages, model

def model_response_generator(model, messages):
    stream = ollama.chat(
        model=model,
        messages=messages,
        stream=True,
    )
    for chunk in stream:
        yield chunk["message"]["content"]

def main():
    # Задаем модель по умолчанию
    engine = pyttsx3.init()
    voices = engine.getProperty('voices')

    for voice in voices:
        engine.setProperty('voice', voices[2].id)

    # Adjust rate, volume, and pitch for better clarity
    engine.setProperty('rate', 100)  # Slow down a bit for clarity
    engine.setProperty('volume', 3.0)  # Maximize volume
    engine.setProperty('pitch', 100)  # Optional: Adjust pitch





    model_name = "llama2"

    messages, model = initialize_chat(model_name)

    while True:
        print("Скажите что-нибудь...")

        # Распознаем речь и записываем в переменную prompt
        prompt = ""
        while True:
            try:
                data = stream.read(4096)
                if recognizer.AcceptWaveform(data):
                    result = recognizer.Result()
                    result_text = eval(result)['text']
                    print("Вы сказали: " + result_text)
                    if "direction" in result_text.lower() or "how do I get to home" in result_text.lower():
                        response = "oh,  Follow  the caterpillar."

                    prompt = result_text
                    break
            except OSError as e:
                if e.errno == -9981:
                    print("Input overflowed. Пожалуйста, говорите медленнее.")
                else:
                    print("Произошла ошибка: ", e)
                continue

        # Add stylistic instruction to the prompt

        styled_prompt = (

            f"Respond to the following query shortly in 1sentence in style of wise tree with a friendly and wise tone. get ready to respond to the query about Wonderland from Lewis Carroll's book Alice's Adventures in Wonderland. without moves, emotions and emojies. and dont write anything look like * quizzical expression*: {prompt}"
        )



        # Add latest message to history
        messages.append({"role": "user", "content": styled_prompt})

        # Get response from the model
        response = ""
        for chunk in model_response_generator(model, messages):
            response += chunk

        # Print response
        print("Assistant:", response)
        engine.say(response)

        engine.runAndWait()

        # Add response to history
        messages.append({"role": "assistant", "content": response})

if _name_ == "_main_":
    try:
        main()
    finally:
        # Останавливаем поток и закрываем ресурсы после завершения
        stream.stop_stream()
        stream.close()
        p.terminate()
