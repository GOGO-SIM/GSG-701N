/*
 * void gsmpUnwrapper(void* recvByte)
 * GSMP(GoSim Message Protocol)의 수신을 지원한다.
 * return : dataPayload 반환
 *
 **/
void* gsmpUnWrapper(void* recvByte)
{
/**
 * [데이터의 포맷을 unwrap한다.]
 * 송수신하는 모든 시스템은 리틀 엔디안 형식을 사용한다고 가정한다.
 * 1. 수신한 데이터를 해석한다.
 * 1-1. 첫 바이트가 StartFlag와 동일한지 확인한다.
 * 1-2. CRC검사를 수행한다. -> 구체화 필요
 * 1-2-1. CRC 검사 실패 시 복구 절차를 수행한다.
 * 2. 목적지 버퍼에 payload를 저장한다 : optional
 */
}
