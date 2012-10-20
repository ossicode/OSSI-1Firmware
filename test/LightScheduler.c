
enum
{
    MAX_EVENTS = 128, UNUSED = -1
};


typedef struct
{
    int id;
    Day day;
    int minuteOfDay;
    int event;
    int randomize;
    int randomMinutes;

} ScheduledLightEvent;

static ScheduledLightEvent eventList[MAX_EVENTS];

// from test driven developement in c