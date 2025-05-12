
void seekerCommunicationTask( void *pvParameters )
{

/**
 * [seeker 데이터 관리 서버에게 요청하여 데이터 수신]
 * -> 서버와 연결되어있다는 가정.
 * 1. 연결된 소켓을 통해 데이터 요청
 * 2. 데이터 수신(block)
 * 3. 데이터 무결성 검사 턔스크를 깨운다
 * 4. 텔레메트리 문자열 생성하여 큐에 삽입
 * 5. 무결성 체크 결과 확인
 * 	5-1. 통과하지 못한 경우 failCount += 1
 * 	5-2. failCount가 5이상인 경우 종말 플래그 활성화
 * 	5-3. ACB 송신 태스크 깨움
 * 6. 휴면(5ms)
 */
}
