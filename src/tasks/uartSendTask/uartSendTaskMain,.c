
void uartSendTask( void *pvParameters )
{

/**
 * [ACB에 Serial 직렬 연결로 데이터 송신]
 * 1. 현재 Status를 확인한다.
 * 1-1. 종말 상태인 경우
 * 1-1-1. 메세지를 전송한다.
 * 1-1-2. 텔레메트리 문자열 생성하여 큐에 삽입한다.
 * 3. 모터 제어 데이터를 전송한다.
 * send(stat, whereToMoveX, whereToMoveY, whereToMoveZ);
 */
}
