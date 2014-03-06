from pymongo import MongoClient
import time
tik = time.time()

client = MongoClient()
db = client.db

# Make some collections!
# And clear out any old info from debugging!
actors = db.actors_collection
movies = db.movies_collection
actors.remove()
movies.remove()

actorsPosts = []
moviesPosts = []

file = open("actors.txt", "r")
i = 0
newActor = False
actor = ""
actorMovies = []
AllMovies = {}

for line in file:
	line = line.strip()
	linesplit = line.split()

	# Check if we're ready to start a new actor
	if line == "" and actor != "":
		newActor = True
		# If appropriate, post actor data.
		if actorMovies != []:
			actorPost = {"name"      : actor            ,
					 "movies"    : actorMovies      ,
					 "numMovies" : len(actorMovies) }
			#print actorPost
			#actors.insert(actorPost)
			actorsPosts.append(actorPost)
		actorMovies = []
		continue

	# Check whether we're still in the early bits
	if (linesplit == ['----', '------']):
		actor = "ready for first actor"
		newActor = True
		continue
	elif (newActor == False) and (actor == ""):
		continue
	
	# If there's a new actor, grab his/her name
	if newActor:
		try:
			ind = line.index("\t")
		except ValueError: # We're done!!!
			break
		actor = line[:ind].decode('latin9').encode('utf-8')
		newActor = False
		line = line[ind:]
		linesplit = line.split()

	# Strip it down to the name of the movie
	# so remove the year it came out, the character name, etc
	# i.e., everything after the (
	# It's possible that a movie title will have ( in it,
	# but it'll get stripped out for everyone equally, so it's ok
	try:
		ind = line.index("(")
	except ValueError:
		print "ERROR: %s" % line.split()
		break
	movie = line[:ind].strip().decode('latin9').encode('utf-8')
	
	# avoid duplicates
	if movie in actorMovies:
		continue
	actorMovies.append(movie)

	if movie not in AllMovies:
		AllMovies[movie] = set()
	AllMovies[movie].add(actor)

	### DATABASE BUSINESS ###
	# If the movie post doesn't already exist, make one.
	#if movies.find_one({"name":movie}) == None:
	#	movies.insert( {"name"   :   movie,
	#					"actors" :   []   ,
	#					"numActors" : 0   })
	#movieObj = movies.find_one({"name":movie})
	# Update the list of actors.
	#movies.update({"name":movie},
	#			  {"actors":movieObj["actors"].append(actor),
	#			   "numMovies":len(movieObj["actors"])+1})
	# Update the number of actors.
	i+=1
	if i%10000==0:
		tok = time.time()
		print "Num:", i, "Time", (tok-tik), "Expected", (15000000./i*(tok-tik)), "AllMovies", len(AllMovies)

print "All actors processed."
print "Processing movies..."

for movie in AllMovies:
	moviesPosts.append({"name"      : movie                 ,
						"actors"    : list(AllMovies[movie]),
						"numActors" : len(AllMovies[movie]) })

print "All movies processed."
print "Pushing movies..."
movies.insert(moviesPosts)
print "Done!"
print "Pushing actors..."
i = 0
l = len(actorsPosts)
for post in actorsPosts:
	actors.insert(post)
	i+=1
	if i % 2000 == 0:
		print i, "of", l

tok = time.time()
print "Time elapsed: %fs" % (tok - tik)