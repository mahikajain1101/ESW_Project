from flask import Flask,render_template,url_for,request,redirect, make_response
import random
import json
import numpy as np
import urllib.request
import requests
import threading
import subprocess
import calendar
import pickle
from time import time
from random import random
from datetime import datetime
import jinja2
from flask import Flask, render_template, make_response
app = Flask(__name__)

with open('new.pkl','rb') as file:
    model = pickle.load(file)


@app.route('/', methods=["GET", "POST"])


def main():

    test = np.array([0.5,0.9,0.1,0.6,0.4,0.5,0.8,0.2,0.5])
    test = np.expand_dims(test,0)
    pred = model.predict(test)
    r = requests.get("https://api.thingspeak.com/channels/1921492/fields/1/last.txt")
    current = r.text
    return render_template('indexHello.html', curr=current,result=pred)

# @app.route('/get_plot_data', methods=["GET", "POST"])
# def get_plot_data():
#     code , result = get_data("https://esw-onem2m.iiit.ac.in/~/in-cse/in-name/Team-33/Node-1/Data/la")
#     encrypted_string = result['m2m:cin']['con']
#     # print(string)

#     # remove the '[' from the string
#     string = string[1:]
#     # remove the ' ' from the string
#     string = string.replace(" ", "")
#     # store the , seperated number from string in a list and convert them to integer
#     string = [int(i) for i in string.split(',')]
#     response = make_response(json.dumps(string))
#     response.content_type = 'application/json'
#     return response

if __name__ == "__main__":
    app.run()