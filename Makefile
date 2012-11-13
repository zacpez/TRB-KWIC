# Makefile to manage the master Git repository
# Version 1.0
# Developed by: Team Raptor Bunnies

# Targets
#    remotes   --- sets up repositories
#    pullall   --- gets all changes from all repositories
#    upload 	--- sends local master repository to Github

remotes:
	git remote add zac ../../zac/TRB-KWIC/
	git remote add rob ../../rob/TRB-KWIC/
	git remote add alan ../../alan/TRB-KWIC/
	git remote add dustin ../../dustin/TRB-KWIC/
	git remote add nicole ../../nicole/TRB-KWIC/
	git remote add Angela ../../ang/TRB-KWIC/

pullall:
	git pull zac master
	git pull rob master
	git pull alan master
	git pull dustin master
	git pull nicole master
	git pull Angela master

upload:
	git push origin master





