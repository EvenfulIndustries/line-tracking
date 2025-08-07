#define OLD_EVENT_THRESHOLD 40 miliseconds SOMEHOW 
#define LINE_LENGTH_MINIMUM 70 //pixels
#define MAX_EVENTS_IN_LINE 100
#define MAX_EVENTS_IN_CLUSTER 100
#define NULL_EVENT_TIMESTAMP 0 //decides the timestamp for the null event.

//EXPLANATION: cause we cant null an entry of an array in C, any events with the timestamp 0 are considered null & gone.


int32[][] SAE = new int32[640][480];

struct event
{
	int32 timestamp,
	int16 x,
	int16 y
};

struct cluster
{
	event[] containedEvents = new event[MAX_EVENTS_IN_CLUSTER] //arbitrary
	int16 inferredLineLength, //needs to be at least 640
	int16 inferredLineAngleHough, //needs to be capable of holding 360
	int16 inferredLineDistanceHough //needs to be capable of holding 640/2
};

struct line
{
	event[] containedEvents = new event[MAX_EVENTS_IN_LINE] //arbitrary
	int16 lineLength, //needs to be at least 640.
	int16 lineAngleHough, //needs to be capable of holding 360
	int16 lineDistanceHough, //needs to be capable of holding 640/2
	int8 state //0 = null, 1 = initializing, 2 = active, 3 = hibernating
};

cluster[] clusterArray = new cluster[20];
line[] lineArray = new cluster[20];

int runPeriodicTicker = 0;
int runPeriodicLimit = 5;


void OnReceiveEvent(event receivedEvent)
{
	//first check if it's garbage day
	if (runPeriodicTicker == runPeriodicLimit)
	{
		runPeriodicTicker = 0;
		periodicCleanup(receivedEvent); //needed to know current timestamp
	}
	else
		runPeriodicTicker++;
	
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

void periodicCleanup(event latestEvent)
{
	//LINES

	for (int i = 0 ; i < lineArray.length, i++)
	{
		//IF line is null, ignore
		if (lineArray[i].state = 0)
			continue;

		//run through contained events in each line
		for (int j = 0 ; j < lineArray[i].containedEvents.length, j++)
		{
			//REMOVE OLD EVENTS
			if (latestEvent.timestamp - lineArray[i].containedEvents[j].timestamp > OLD_EVENT_THRESHOLD)
				lineArray[i].containedEvents[j].timestamp = 0; //null the event
			//UPDATE PLANE ESTIMATE
			updatePlaneEstimate(lineArray[i]);
			
			//DELETE LINES TOO SHORT
			if (lineArray[i].lineLength < LINE_LENGTH_LIMIT)
				lineArray[i].state = 0; //null the line
			
			
			
		}

	}
}