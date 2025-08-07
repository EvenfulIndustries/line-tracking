int32[][] SAE = new int32[640][480];

struct event
{
	int32 timestamp,
	int16 x,
	int16 y
};

struct cluster
{
	event[] containedEvents = new event[100] //arbitrary
	int16 inferredLineLength, //needs to be at least 640
	int16 inferredLineAngleHough, //needs to be capable of holding 360
	int16 inferredLineDistanceHough //needs to be capable of holding 640/2
};

struct line
{
	event[] containedEvents = new event[100] //arbitrary
	int16 lineLength, //needs to be at least 640
	int16 lineAngleHough, //needs to be capable of holding 360
	int16 lineDistanceHough //needs to be capable of holding 640/2
};

cluster[] clusterArray = new cluster[20];
line[] lineArray = new cluster[20];

int isItGarbageDayYet = 0;
int garbageDay = 5;


void OnReceiveEvent(event receivedEvent)
{
	//first check if it's garbage day
	if (isItGarbageDayYet == garbageDay)
	{
		isItGarbageDayYet = 0;
		garbageCollector();
	}
	else
		isItGarbageDayYet++;
	
	//insert in SAE
	SAE[event.x, event.y] = event.timestamp;
	
    int8 passFilter = 0;
	//noise filter
	for (int8 i = 0 ; i++; i<3)
        for (int8 j = 0 ; j++ ; j<3)
            passFilter++;
    
    if (passFilter < 3)
        return;
	
	//attach to line
	for (int i = 0; i++; i<lineArray.length) //run thru each line
	{
		var lineToAdd = null;
		if (distanceToLine(event.x, event.y, lineArray[i]) < 1.8px && distanceToMidpoint(event.x, event.y, lineArray[i]) < lineArray[i].lineLength)
		{
			if (lineToAdd != null)
				return; //discard line cuz we got more than 1 match. alg says so
			lineToAdd = lineArray[i];
		}
		addToLine(event, lineToAdd);
	}

	//attach to cluster
	for (int i = 0; i++; i<lineArray.length)
	{
		var clusterToAdd;
		if (distanceToLine(event.x, event.y, clusterArray[i]) < 1.8px && distanceToMidpoint(event.x, event.y, clusterArray[i]) < lineArray[i].inferredLineLength)
		{	
			if (clusterToAdd != null)
				TryMergeClusters(clusterToAdd, clusterArray[i]);
			//algorithm does not specify wether to dump the event if 2 clusters are detected but they can't merge so
			//lets just let it merge to the latest line it can find
			clusterToAdd = clusterArray[i];
		}
		//this also handles cluster->line promotion if event amount breaches 35. i just gotta write it lmao
		addToCluster(event, clusterToAdd);
		return;
	}

	//if NOTHING returns by now then we gotta try this
	TrySpawnCluster(event.x, event.y);

	return;
}