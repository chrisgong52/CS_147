from flask import Flask, render_template
from flask import request
app = Flask(__name__)

# global k_tip
# global k_health
# global k_day
# global k_dark
# global k_temp

k_tip = -1
k_health = -1
k_sunlight = -1
k_dark = -1
k_temp = -1
sun_min = -1
sun_sec = -1
dark_min = -1
dark_sec = -1
sun_pass = ""
dark_pass = ""

@app.route("/", methods=["GET", "POST"])
def hello():
    global k_tip
    global k_health
    global k_sunlight
    global k_dark
    global k_temp
    global sun_min
    global sun_sec
    global dark_min
    global dark_sec
    global sun_pass
    tip = k_tip
    health = k_health
    sunlight = k_sunlight
    dark = k_dark
    temp = k_temp
    # print(request.args.get("var"))
    if request.method == "POST":
        print("received post from page")
    if request.method == "GET":
        print("received get from page")
    temp_lower_thresh = 70
    hum_lower_thresh = 20
    tip = request.args.get("tip") if request.args.get("tip") != None else tip
    if tip == "true":
        tip = "Tipped"
    else:
        tip = "Standing"
    k_tip = request.args.get("tip") if request.args.get("tip") != None else k_tip
    print("tip: ", tip)
    health = request.args.get("health") if request.args.get("health") != None else health
    k_health = request.args.get("health") if request.args.get("health") != None else k_health
    print("health: ", health)
    sunlight = int(request.args.get("day")) if request.args.get("day") != None else sunlight
    k_sunlight = int(request.args.get("day")) if request.args.get("day") != None else k_sunlight

    if sunlight != -1:
        sun_min = sunlight//60
        sun_sec = sunlight%60
        if sun_min < 10:
            sun_min = "0" + str(sun_min)
        if sun_sec < 10:
            sun_sec = "0" + str(sun_sec)

    sun_pass = str(sun_min) + ":" + str(sun_sec)

    print("sunlight: ", sunlight)
    print("formatted sunlight: ", sun_pass)
    dark = int(request.args.get("darkness")) if request.args.get("darkness") != None else dark
    k_dark = int(request.args.get("darkness")) if request.args.get("darkness") != None else k_dark

    if sunlight != -1:
        dark_min = dark//60
        dark_sec = dark%60
        if dark_min < 10:
            dark_min = "0" + str(dark_min)
        if dark_sec < 10:
            dark_sec = "0" + str(dark_sec)

    dark_pass = str(dark_min) + ":" + str(dark_sec)
    print("dark: ", dark)
    print("formatted dark: ", dark_pass)
    temp = request.args.get("temp") if request.args.get("temp") != None else temp
    k_temp = request.args.get("temp") if request.args.get("temp") != None else k_temp
    print("temp: ", temp)
    return render_template("page.html", tip = tip, health = health, sunlight = sun_pass, dark = dark_pass, temp = "%.2f" % float(temp))