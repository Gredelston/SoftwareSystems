from pymongo import MongoClient
import sys

def actorGet(name):
	global actors
	return actors.find_one({'name':name})

def movieGet(name):
	global movies
	return movies.find_one({'name':name})

# Check whether two actors are in the same movie. Return True if so, return false if not.
def actorCheck(actorObj, compareActorObj):
	found = False
	for movie in actorObj['movies']:
		if movie in compareActorObj['movies']:
			found = True
			print actorObj['name'] + " and " + compareActorObj['name'] + " are both in " + movie  

		#checkMovies[] = movie
	return found
#Takes a movie and an actor and checks if any actors in the movie are also in a movie with given actor
def movieCheck(movie, compareActorObj):
	
	movieObj = movieGet(movie)
	for actor in movieObj['actors']:
		if actor not in doneActors:
			if actorCheck(actorGet(actor), compareActorObj):
				return [], True
	return movieObj['actors'], False


#set up Mongo
client = MongoClient('localhost', 27017)
db = client.softsys
actors = db.actors
movies = db.movies

# End condition parameters
BaconNum = 0
found = False

#get actors
actor1  = sys.argv[1].lower()
actor2  = sys.argv[2].lower()
actor1Obj = actorGet(actor1)
actor2Obj = actorGet(actor2)

doneMovies = []

checkMovies = []
doneActors = []

print "Looking for connection between " +actor1 +" and " + actor2

#mv = actors.find({"movies":["olin","gann"]})
#print mv[0]
# First check if actors in the same movie. Bacon ++

if not actorCheck(actor1Obj, actor2Obj):
	BaconNum += 1
	checknext = []
	for movie in actor1Obj['movies']:
		currchecknext, found = movieCheck(movie, actor2Obj)
		checknext = checknext + currchecknext




#DON't GO PAST HERE< THAR BE DRAGONS AND NOT ACTUAL CODE



	# if movie in a
	# ctor2Obj['movies']:
	# 		found = True
	# 		print actor2 + " and " + actor1 " are both in " + movie  
	# 	checkMovies[] = movie
	# for movie in actor1Obj['movies']:
	# 	actorList = movies.find_one({"name": movie})


#check for each 
