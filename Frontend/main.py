# for wrong termination "sudo lsof -nPi :5000" - find process running on port 5000
# for killing the process "sudo kill -9 *PID of 5000*" - kill process  using port 5000

from website import create_app

app = create_app()

if __name__ == "__main__":
    app.run(debug=True)