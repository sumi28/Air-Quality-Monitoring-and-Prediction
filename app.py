# -*- coding: utf-8 -*-

 

from sklearn import linear_model
from sklearn import tree, svm, ensemble
import numpy as np
import pandas as pd
from sklearn.model_selection import train_test_split
from sklearn.ensemble import RandomForestClassifier
import matplotlib.pyplot as plt 
import seaborn 
from flask import Flask, render_template, request




app = Flask(__name__,static_url_path='/static')
data=pd.read_csv(r"C:\Users\lenovo\Desktop\Be proj\DA.csv")
date_map ={"Monday":1,"Tuesday":2,"Wednesday":3,"Thursday":4,"Friday":5,"Saturday":6, "Sunday":7}
data.loc[:, "Date"]= data.loc[:, "Date"].map(date_map)
loct_map = {"Chembur":1,"Kurla":2,"Turbhe":3,"Vashi":4,"Panvel":5,"Ulhas":6, "Thane":7, "Khar":8}
data.loc[:, "Location"]= data.loc[:, "Location"].map(loct_map)
@app.route('/')
def index():
    return render_template('index.html')


@app.route('/sendco', methods=['GET','POST'])
def sendco():
    if request.method == 'POST' :
       
        Date = request.form['day'];
        Time=request.form['time'];
        Location=request.form['location'];
                
        #For CO
        threshold=50
        pollutant='CO(in ppm)'
        regR=ensemble.GradientBoostingRegressor()
        regR.fit(data[['Location','Date','Time']], data.CO)
        answer = regR.predict([[Location,Date,Time]])
    if answer<50: 
        pol='LOW POLLUTION'
    else:
        pol='HIGH POLLUTION'
    return render_template('day.html',day=Date,time=Time,location=Location,threshold=threshold,predict=answer,disp=pol,pollutant=pollutant)
        
        
@app.route('/sendcot', methods=['GET','POST'])
def sendcot():
    if request.method == 'POST' :
        Date = request.form['day'];
        Time=request.form['time'];
        Location=request.form['location'];
        #For CO2
        threshold=1000
        pollutant='CO2(in ppm)'
        regG=ensemble.GradientBoostingRegressor()
        regG.fit(data[['Location','Date','Time']], data.CO2)
        answerc = regG.predict([[Location,Date,Time]])
    if answerc<1000: 
        polc='LOW POLLUTION'
    else:
        polc='HIGH POLLUTION'
    return render_template('day.html',day=Date,time=Time,threshold=threshold,location=Location,predict=answerc,disp=polc,pollutant=pollutant)
        


@app.route('/sendno', methods=['GET','POST'])
def sendno():
    if request.method == 'POST' :
        Date = request.form['day'];
        Time=request.form['time'];
        Location=request.form['location'];
        #For NO2
        pollutant='Smoke(in ppm)'
        threshold=100
        regN=ensemble.GradientBoostingRegressor()
        regN.fit(data[['Location','Date','Time']], data.NO2)
        answern = regN.predict([[Location,Date,Time]])
    if answern<100: 
        poln='LOW POLLUTION'
    else:
        poln='HIGH POLLUTION' 
    


    return render_template('day.html',day=Date,time=Time,location=Location,threshold=threshold,predict=answern,disp=poln,pollutant=pollutant)
    
if __name__=="__main__":
    app.run(debug=True,port=4000)






