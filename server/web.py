from flask import Flask, render_template
import os

PEOPLE_FOLDER = os.path.join('static', 'imgs')

app = Flask(__name__)
app.config['UPLOAD_FOLDER'] = PEOPLE_FOLDER

@app.route('/')
@app.route('/index/<string:name>')
def show_index(name):
    file_name = name+'.jpg'
    file_name = 'imgs/'+file_name
    return render_template("index.html", user_image = file_name)
    

if __name__ == '__main__':
    app.run()