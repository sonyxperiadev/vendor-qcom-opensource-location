/* Copyright (c) 2018-2021, The Linux Foundation. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above
 *       copyright notice, this list of conditions and the following
 *       disclaimer in the documentation and/or other materials provided
 *       with the distribution.
 *     * Neither the name of The Linux Foundation nor the names of its
 *       contributors may be used to endorse or promote products derived
 *       from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED "AS IS" AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS
 * BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR
 * BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
 * OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN
 * IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/*
Changes from Qualcomm Innovation Center are provided under the following license:

Copyright (c) 2022 Qualcomm Innovation Center, Inc. All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted (subject to the limitations in the
disclaimer below) provided that the following conditions are met:

    * Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.

    * Redistributions in binary form must reproduce the above
      copyright notice, this list of conditions and the following
      disclaimer in the documentation and/or other materials provided
      with the distribution.

    * Neither the name of Qualcomm Innovation Center, Inc. nor the names of its
      contributors may be used to endorse or promote products derived
      from this software without specific prior written permission.

NO EXPRESS OR IMPLIED LICENSES TO ANY PARTY'S PATENT RIGHTS ARE
GRANTED BY THIS LICENSE. THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT
HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED
WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE
GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER
IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN
IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#ifndef LOCATIONCLIENTAPIIMPL_H
#define LOCATIONCLIENTAPIIMPL_H

#include <mutex>

#include <loc_pla.h>
#include <LocIpc.h>
#include <ILocationAPI.h>
#include <MsgTask.h>
#include <LocationApiMsg.h>
#include <LocationApiPbMsgConv.h>
#include <LCAReportLoggerUtil.h>
#ifdef NO_UNORDERED_SET_OR_MAP
    #include <set>
    #include <map>
    #define unordered_set set
    #define unordered_map map
#else
    #include <unordered_set>
    #include <unordered_map>
#endif

using namespace std;
using namespace loc_util;

namespace location_client
{

enum ReportCbEnumType {
    REPORT_CB_TYPE_NONE   = 0,
    /** cb for GNSS info, including location, sv info, nmea and
     *  etc */
    REPORT_CB_GNSS_INFO   = 1,
    /** cb for GNSS info, including location, sv info, nmea and
     *  etc and also for location of other engines running in the
     *  system */
    REPORT_CB_ENGINE_INFO = 2,
};

typedef std::function<void(
    uint32_t response
)> PingTestCb;


class GeofenceImpl: public std::enable_shared_from_this<GeofenceImpl> {
    uint32_t mId;
    Geofence mGeofence;
    static uint32_t nextId();
    static mutex mGfMutex;
public:
    GeofenceImpl(Geofence* geofence) : mId(nextId()), mGeofence(*geofence) {
    }
    void bindGeofence(Geofence* geofence) {
        geofence->mGeofenceImpl = shared_from_this();
    }
    inline uint32_t getClientId() { return mId; }
};

class IpcListener;

class LocationClientApiImpl : public ILocationAPI {
    friend IpcListener;
public:
    LocationClientApiImpl(capabilitiesCallback capabitiescb);
    virtual void destroy(locationApiDestroyCompleteCallback destroyCompleteCb=nullptr) override;

    // Tracking
    virtual void updateCallbacks(LocationCallbacks&) override;

    virtual uint32_t startTracking(TrackingOptions&) override;

    virtual void stopTracking(uint32_t id) override;

    virtual void updateTrackingOptions(uint32_t id, TrackingOptions&) override;

    //Batching
    virtual uint32_t startBatching(BatchingOptions&) override;

    virtual void stopBatching(uint32_t id) override;

    virtual void updateBatchingOptions(uint32_t id, BatchingOptions&) override;

    virtual void getBatchedLocations(uint32_t id, size_t count) override;

    //Geofence
    virtual uint32_t* addGeofences(size_t count, GeofenceOption*,
                                   GeofenceInfo*) override;

    virtual void removeGeofences(size_t count, uint32_t* ids) override;

    virtual void modifyGeofences(size_t count, uint32_t* ids,
                                 GeofenceOption* options) override;

    virtual void pauseGeofences(size_t count, uint32_t* ids) override;

    virtual void resumeGeofences(size_t count, uint32_t* ids) override;

    //GNSS
    virtual void gnssNiResponse(uint32_t id, GnssNiResponse response) override;

    // other interface
    void startPositionSession(const LocationCallbacks& callbacksOption,
                              const TrackingOptions& trackingOptions);

    void startBatchingSession(const LocationCallbacks& callbacksOption,
                              const BatchingOptions& batchOptions);

    void updateNetworkAvailability(bool available);
    void getGnssEnergyConsumed(gnssEnergyConsumedCallback gnssEnergyConsumedCb,
            responseCallback responseCb);
    void updateLocationSystemInfoListener(
            locationSystemInfoCallback locationSystemInfoCb,
            responseCallback responseCb);

    void addGeofences(const LocationCallbacks& callbacksOption,
                      const std::vector<Geofence>& geofences);
    inline Geofence getMappedGeofence(uint32_t id) {
        return mGeofenceMap.at(id);
    }

    inline uint16_t getYearOfHw() {return mYearOfHw;}

    void getSingleTerrestrialPos(uint32_t timeoutMsec, TerrestrialTechMask techMask,
                                 float horQoS, trackingCallback terrestrialPositionCallback,
                                 responseCallback responseCallback);

    void pingTest(PingTestCb pingTestCallback);

    static LocationSystemInfo parseLocationSystemInfo(
            const::LocationSystemInfo &halSystemInfo);
    static LocationResponse parseLocationError(::LocationError error);
    static GnssMeasurements parseGnssMeasurements(const ::GnssMeasurementsNotification
            &halGnssMeasurements);
    static GnssData parseGnssData(const ::GnssDataNotification &halGnssData);
    static GnssSv parseGnssSv(const ::GnssSv &halGnssSv);
    static GnssLocation parseLocationInfo(const ::GnssLocationInfoNotification &halLocationInfo);
    static GnssSystemTime parseSystemTime(const ::GnssSystemTime &halSystemTime);
    static GnssGloTimeStructType parseGloTime(const ::GnssGloTimeStructType &halGloTime);
    static GnssSystemTimeStructType parseGnssTime(const ::GnssSystemTimeStructType &halGnssTime);
    static LocationReliability parseLocationReliability(
            const ::LocationReliability &halReliability);
    static GnssLocationPositionDynamics parseLocationPositionDynamics(
            const ::GnssLocationPositionDynamics &halPositionDynamics,
            const ::GnssLocationPositionDynamicsExt &halPositionDynamicsExt);
    static void parseGnssMeasUsageInfo(const ::GnssLocationInfoNotification &halLocationInfo,
            std::vector<GnssMeasUsageInfo>& clientMeasUsageInfo);
    static GnssSignalTypeMask parseGnssSignalType(
            const ::GnssSignalTypeMask &halGnssSignalTypeMask);
    static GnssLocationSvUsedInPosition parseLocationSvUsedInPosition(
            const ::GnssLocationSvUsedInPosition &halSv);
    static void parseLocation(const ::Location &halLocation, Location& location);
    static Location parseLocation(const ::Location &halLocation);
    static uint16_t parseYearOfHw(::LocationCapabilitiesMask mask);
    static LocationCapabilitiesMask parseCapabilitiesMask(::LocationCapabilitiesMask mask);
    static GnssMeasurementsDataFlagsMask parseMeasurementsDataMask(
            ::GnssMeasurementsDataFlagsMask in);
    static GnssEnergyConsumedInfo parseGnssConsumedInfo(::GnssEnergyConsumedInfo);

    void logLocation(const GnssLocation &gnssLocation);
    LCAReportLoggerUtil & getLogger() {
        return mLogger;
    }

private:
    ~LocationClientApiImpl();

    inline LocationCapabilitiesMask getCapabilities() {return mCapsMask;}
    void capabilitesCallback(ELocMsgID  msgId, const void* msgData);
    void updateTrackingOptionsSync(TrackingOptions& option);
    bool checkGeofenceMap(size_t count, uint32_t* ids);
    void addGeofenceMap(uint32_t id, Geofence& geofence);
    void eraseGeofenceMap(size_t count, uint32_t* ids);

    // convenient methods
    inline bool sendMessage(const uint8_t* data, uint32_t length) const {
        return (mIpcSender != nullptr) && LocIpc::send(*mIpcSender, data, length);
    }

    void invokePositionSessionResponseCb(LocationError errCode);
    void diagLogGnssLocation(const GnssLocation &gnssLocation);

    // protobuf conversion util class
    LocationApiPbMsgConv mPbufMsgConv;

    // internal session parameter
    static uint32_t         mClientIdGenerator;
    static uint32_t         mClientIdIndex;
    static mutex            mMutex;
    uint32_t                mClientId;
    uint32_t                mSessionId;
    uint32_t                mBatchingId;
    bool                    mHalRegistered;
    // For client on different processor, socket name will start with
    // defined constant of SOCKET_TO_EXTERANL_AP_LOCATION_CLIENT_BASE.
    // For client on same processor, socket name will start with
    // SOCKET_LOC_CLIENT_DIR + LOC_CLIENT_NAME_PREFIX.
    char                       mSocketName[MAX_SOCKET_PATHNAME_LENGTH];
    // for client on a different processor, 0 is invalid
    uint32_t                   mInstanceId;
    LocationCallbacksMask      mCallbacksMask;
    LocationOptions            mLocationOptions;
    BatchingOptions            mBatchingOptions;
    LocationCapabilitiesMask   mCapsMask;
    //Year of HW information, 0 is invalid
    uint16_t                   mYearOfHw;
    bool                       mPositionSessionResponseCbPending;
    uint64_t                   mSessionStartBootTimestampNs;

    // callbacks
    LocationCallbacks       mLocationCbs;

    //TODO:: remove after replacing all calls with ILocationAPI callbacks
    capabilitiesCallback    mCapabilitiesCb;
    PingTestCb              mPingTestCb;

    gnssEnergyConsumedCallback    mGnssEnergyConsumedInfoCb;
    responseCallback              mGnssEnergyConsumedResponseCb;

    locationSystemInfoCallback    mLocationSysInfoCb;
    responseCallback              mLocationSysInfoResponseCb;

    // Terrestrial fix callback
    trackingCallback              mSingleTerrestrialPosCb;
    responseCallback              mSingleTerrestrialPosRespCb;

    MsgTask                    mMsgTask;

    LocIpc                     mIpc;
    shared_ptr<LocIpcSender>   mIpcSender;

    std::vector<uint32_t>      mLastAddedClientIds;
    // clientId --> Geofence object
    std::unordered_map<uint32_t, Geofence> mGeofenceMap;

    LCAReportLoggerUtil        mLogger;
};

} // namespace location_client

#endif /* LOCATIONCLIENTAPIIMPL_H */
