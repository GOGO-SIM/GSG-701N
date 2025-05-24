/*
 * void* gsmpWrapper(int messageId, int messageStatus, void* pPayload, int destId)
 * GSMP(GoSim Message Protocol)의 전송을 지원한다.
 * return : ByteStream 반환
 *
 **/
void* gsmpWrapper(int messageId, int messageStatus, void* pPayload, int destId)
{
/**
 * [데이터의 포맷을 wrap한다.]
 * 송수신하는 모든 시스템은 리틀 엔디안 형식을 사용한다고 가정한다.
 * 1. 인자값을 바탕으로 바이트 스트림을 생성한다.
 * 2. 생성한 바이트스트림을 반환한다.
 */
}
