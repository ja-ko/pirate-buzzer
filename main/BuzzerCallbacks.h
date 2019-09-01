
#ifndef COMPONENTS_MAIN_BUZZERCALLBACKS_H_
#define COMPONENTS_MAIN_BUZZERCALLBACKS_H_

#include "events.h"

class BuzzerCallbacks {
public:
	virtual ~BuzzerCallbacks() {};

    virtual void onClientConnected() {};
    virtual void onClientAuthenticated() {};
    virtual void onClientDisconnected() {};
    virtual void onBuzzerDown() {};
    virtual void onBuzzerUp() {};
    virtual void onExternalEvent(buzzer_remote_event_data_t* data) {};
protected:

private:

}; // BuzzerCallbacks
#endif /* COMPONENTS_MAIN_BUZZERCALLBACKS_H_ */
