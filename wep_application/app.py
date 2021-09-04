import slh_solver

from os import listdir, remove, rename
from flask import Flask, render_template, jsonify, request
from multiprocessing import Array
from datetime import datetime

app = Flask(__name__)
status_keys = ["status","stage","vertices","gaps","timgs","imgs","time"]
status_values = Array('i',[0]*len(status_keys))

@app.route("/")
def index():
    return render_template("index.html")

@app.route("/upload", methods=['POST'])
def upload():
    request.files['file'].save("./static/graph/graph.hcp")
    return '',204

@app.route("/solve")
def solve():
    for i in range(len(status_keys)): status_values[i] = 0
    for img in listdir("./static/img/"): remove("./static/img/"+img)
    solver = slh_solver.solver("./static/graph/graph.hcp",status_values)
    solver.solve()

    return "",204

@app.route("/status")
def status():
    status_dict = dict(zip(status_keys,status_values))
    if status_values[0]==4:
        dtstr = datetime.now().strftime("%d-%m-%Y-%H-%M-%S-")
        for img in listdir("./static/img/"): rename("./static/img/"+img,"./static/img/"+dtstr+img)
        status_dict["dtstr"] = dtstr
        
    return jsonify(status_dict)

if __name__ == '__main__':
   app.run(debug = True)
