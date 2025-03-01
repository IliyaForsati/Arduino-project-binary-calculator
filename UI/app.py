import tkinter as tk
from tkinter import ttk
import serial
import threading

def keypress_handler(key):
    print(f"Key pressed: {key}")
    encoded_message = f"K<{key}>\n"
    arduino.write(encoded_message.encode())

def change_textbox_text(new_text):
    textbox_var.set(new_text)

def on_keypress(key):
    keypress_handler(key)

def read_from_serial():
    """Continuously read data from the serial port."""
    while True:
        if arduino.in_waiting > 0:  # Check if there's data available to read
            data = arduino.readline().decode().strip()  # Read and decode the data
            print(f"Received: {data}")  # Print the received data
            if data.startswith("R<") and data.endswith(">"):
                received_text = data[2:-1]  # Extract the content inside R<>
                change_textbox_text(received_text)

# Initialize serial communication (adjust the port and baud rate as needed)
arduino = serial.Serial(port='/dev/cu.usbserial-1120', baudrate=9600, timeout=1)  # Replace with your actual port

# Start a thread to read serial data
serial_thread = threading.Thread(target=read_from_serial, daemon=True)
serial_thread.start()

# Create the main window
root = tk.Tk()
root.title("Calculator UI")
root.geometry("400x600")
root.resizable(False, False)
root.configure(bg="#1e1e2e")

# Textbox to display something
textbox_var = tk.StringVar(value="Welcome!")
textbox = tk.Entry(root, textvariable=textbox_var, font=("Papyrus", 26, "bold"), justify="left", bg="#3b3b4f", fg="#e2e2e3", bd=10, relief="flat")
textbox.grid(row=0, column=0, columnspan=4, padx=10, pady=20, sticky="nsew")

# Define the keys
keys = [
    '0', '1', '(', ')',
    '+', '-', '*', '/',
    '=', 'R'
]

# Create a frame for buttons
button_frame = tk.Frame(root, bg="#1e1e2e")
button_frame.grid(row=1, column=0, columnspan=4, padx=10, pady=10, sticky="nsew")

# Add buttons to the frame
button_colors = ["#ff6f61", "#ffa500", "#ffd700", "#98fb98", "#87ceeb", "#ba55d3", "#ff4500", "#4682b4", "#00fa9a", "#d2691e"]
for index, (key, color) in enumerate(zip(keys, button_colors)):
    btn = tk.Button(
        button_frame,
        text=key,
        command=lambda k=key: on_keypress(k),
        font=("Papyrus", 18, "bold"),
        bg=color,
        fg="#ffffff",
        activebackground="#e2e2e3",
        activeforeground="#000000",
        bd=0,
        relief="flat"
    )
    btn.grid(row=index // 4, column=index % 4, padx=8, pady=8, sticky="nsew")

# Configure button frame rows and columns to expand evenly
for i in range((len(keys) + 3) // 4):  # Adjust rows dynamically based on key count
    button_frame.rowconfigure(i, weight=1)

for j in range(4):  # Number of columns (4 buttons per row)
    button_frame.columnconfigure(j, weight=1)

# Configure root rows and columns to expand evenly
root.rowconfigure(0, weight=1)
root.rowconfigure(1, weight=4)
root.columnconfigure(0, weight=1)

# Example usage of `change_textbox_text`
change_textbox_text("Hello, Calculator!")

# Run the application
root.mainloop()