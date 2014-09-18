/**
 *
 * Copyright 2009 Neuron Robotics, LLC
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */
#include "Bowler/Bowler.h"
#if defined(__PIC32MX__)
extern MAC_ADDR MyMAC __attribute__((section(".scs_global_var")));
#else
extern MAC_ADDR MyMAC;
#endif
//static uint8_t i;

void LoadCorePacket(BowlerPacket * Packet) {
    //SetColor(0,1,0);
    uint8_t i;
    Packet->use.head.ProtocolRevision = BOWLER_VERSION;
    for (i = 0; i < 6; i++) {
        Packet->use.head.MAC.v[i] = 0;
    }
    Packet->use.head.MessageID = 1;
    Packet->use.head.ResponseFlag = 1;
    Packet->use.head.Method = BOWLER_STATUS;
    Packet->use.head.RPC = GetRPCValue("****");
    Packet->use.head.DataLegnth = 4;
}

uint8_t CalcCRC(BowlerPacket *Packet) {
    uint16_t v = 0;
    int i;
    for (i = 0; i < 10; i++)
        v += Packet->stream[i];
    return (v & 0x00ff);
}

uint8_t CheckCRC(BowlerPacket *Packet) {
    uint8_t v = CalcCRC(Packet);
    if (Packet->use.head.CRC == v)
        return true;
    return false;
}

void SetCRC(BowlerPacket * Packet) {
    uint8_t v = CalcCRC(Packet);
    Packet->use.head.CRC = v;
}

inline unsigned long GetRPCValue(char * data) {
    UINT32_UNION l;
    l.byte.FB = data[3];
    l.byte.TB = data[2];
    l.byte.SB = data[1];
    l.byte.LB = data[0];
    return l.Val;
}

uint16_t READY(BowlerPacket *Packet, uint8_t code, uint8_t trace) {
    Packet->use.head.Method = BOWLER_STATUS;
    Packet->use.head.RPC = GetRPCValue("_rdy");
    Packet->use.head.MessageID = 0;
    Packet->use.head.DataLegnth = 6;
    Packet->use.data[0] = code;
    Packet->use.data[1] = trace;
    return 6;
}

uint16_t ERR(BowlerPacket *Packet, uint8_t code, uint8_t trace) {
    Packet->use.head.Method = BOWLER_STATUS;
    Packet->use.head.RPC = GetRPCValue("_err");
    Packet->use.head.MessageID = 0;
    Packet->use.head.DataLegnth = 6;
    Packet->use.data[0] = code;
    Packet->use.data[1] = trace;
    return 6;
}

uint16_t SetPacketLegnth(BowlerPacket *Packet, uint8_t len) {
    //Packet.stream[DataSizeIndex]=(BYTE)(len&0x00ff);
    Packet->use.head.DataLegnth = len;
    return len;
}

uint16_t GetPacketLegnth(BowlerPacket *Packet) {
    return BowlerHeaderSize + Packet->use.head.DataLegnth;
}

uint16_t DataLegnthFromPacket(BowlerPacket *Packet) {
    return Packet->use.head.DataLegnth;
}

void copyPacket(BowlerPacket * from, BowlerPacket * to) {
    uint8_t i;
    for (i = 0; i < BowlerHeaderSize + from->use.head.DataLegnth; i++) {
        to->stream[i] = from->stream[i];
    }
}

uint32_t Bytes2Int32(uint8_t a, uint8_t b, uint8_t c, uint8_t d) {
    return ( (((uint32_t) a) << 24) + (((uint32_t) b) << 16) + (((uint32_t) c) << 8) + ((uint32_t) d));
}

uint8_t CheckAddress(uint8_t * one, uint8_t * two) {
    int i;
    for (i = 0; i < 6; i++) {
        if (one[i] != two[i])
            return false;
    }
    return true;
}

float interpolate(INTERPOLATE_DATA * data, float currentTime) {
    float  totalDistance=0, elapsed=0,currentDistance=0, currentLocation=0;
    float set = data->set;
    float start = data->start;
    float setTime = data->setTime;
    float startTime = data->startTime;

    println_I("Time= ");p_fl_I(currentTime);
    println_I("Interpolate data: ");
    print_I(" Set= ");p_fl_I(set);
    print_I(" start= ");p_fl_I(start);
    print_I(" setTime= ");p_fl_I(setTime);
    print_I(" startTime= ");p_fl_I(startTime);
    println_I("From struct:      ");
    print_I(" Set= ");p_fl_I(data->set);
    print_I(" start= ");p_fl_I(data->start);
    print_I(" setTime= ");p_fl_I(data->setTime);
    print_I(" startTime= ");p_fl_I(data->startTime);
    

    elapsed =currentTime- (startTime);
    //interpolation is done
    if(elapsed >= setTime)
        return set;

    totalDistance = set-start;

    // elapsed must always be greater than the set time, current distance will be lower then
    // total distance
    currentDistance = totalDistance * (elapsed/setTime);
    
    // location will be an offset from the start
    currentLocation = currentDistance+start;

//    print_W(" elapsedTime = ");p_fl_W(elapsed);
//    print_W(" incremental distance = ");p_fl_W(currentDistance);
//    print_W(" Target = ");p_fl_W(currentLocation);

    return currentLocation;

}

boolean bound(float target, float actual, float plus, float minus) {
    return ((actual)<(target + plus) && (actual)>(target - minus));
}

void set8bit(BowlerPacket * Packet, uint8_t val, uint8_t offset) {
    Packet->use.data[0 + offset] = val;
}

void set16bit(BowlerPacket * Packet, int16_t val, uint8_t offset) {
    UINT16_UNION wval;
    wval.Val = val;
    Packet->use.data[0 + offset] = wval.byte.SB;
    Packet->use.data[1 + offset] = wval.byte.LB;
}

void set32bit(BowlerPacket * Packet, int32_t val, uint8_t offset) {
    INT32_UNION lval;
    lval.Val = val;
    Packet->use.data[0 + offset] = lval.byte.FB;
    Packet->use.data[1 + offset] = lval.byte.TB;
    Packet->use.data[2 + offset] = lval.byte.SB;
    Packet->use.data[3 + offset] = lval.byte.LB;
}

int32_t get32bit(BowlerPacket * Packet, uint8_t offset) {
    INT32_UNION lval;
    lval.byte.FB = Packet->use.data[0 + offset];
    lval.byte.TB = Packet->use.data[1 + offset];
    lval.byte.SB = Packet->use.data[2 + offset];
    lval.byte.LB = Packet->use.data[3 + offset];
    return lval.Val;
}

int32_t get16bit(BowlerPacket * Packet, uint8_t offset) {
    UINT16_UNION wval;
    wval.byte.SB = Packet->use.data[0 + offset];
    wval.byte.LB = Packet->use.data[1 + offset];
    return wval.Val;
}
