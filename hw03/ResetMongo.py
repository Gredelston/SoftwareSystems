from pymongo import MongoClient

client = MongoClient('localhost', 27017)
db = client.softsys
db.actors.remove()
db.movies.remove()
actors = db.actors
movies = db.movies

movies.insert({"name":"gann", "actors":["shane skikne", "joshi grinberg", "josh canner"], "numActors":3})
movies.insert({"name":"olin", "actors":["shane skikne", "kari bender", "kyle m", "chani martin", "rachel boy"], "numActors":5})
movies.insert({"name":"softartisans", "actors":["shane skikne", "joshi grinberg", "kyle m", "madalyn coryea"], "numActors":4})

actors.insert({"name":"shane skikne", "movies":["gann", "olin", "softartisans"], "numMovies":3})

actors.insert({"name":"joshi grinberg", "movies":["gann", "softartisans"], "numMovies":2})

actors.insert({"name":"josh canner", "movies":["gann"], "numMovies":1})

actors.insert({"name":"kari bender", "movies":[ "olin"], "numMovies":1})

actors.insert({"name":"kyle m", "movies":[ "olin", "softartisans"], "numMovies":2})

actors.insert({"name":"chani martin", "movies":[ "olin"], "numMovies":1})


actors.insert({"name":"rachel boy", "movies":[ "olin"], "numMovies":1})

actors.insert({"name":"madalyn coryea", "movies":[ "softartisans"], "numMovies":1})


