/*
 * void gsmpUnwrapper(void* recvByte)
 * GSMP(GoSim Message Protocol)�� ������ �����Ѵ�.
 * return : dataPayload ��ȯ
 *
 **/
void* gsmpUnWrapper(void* recvByte)
{
/**
 * [�������� ������ unwrap�Ѵ�.]
 * �ۼ����ϴ� ��� �ý����� ��Ʋ ����� ������ ����Ѵٰ� �����Ѵ�.
 * 1. ������ �����͸� �ؼ��Ѵ�.
 * 1-1. ù ����Ʈ�� StartFlag�� �������� Ȯ���Ѵ�.
 * 1-2. CRC�˻縦 �����Ѵ�. -> ��üȭ �ʿ�
 * 1-2-1. CRC �˻� ���� �� ���� ������ �����Ѵ�.
 * 2. ������ ���ۿ� payload�� �����Ѵ� : optional
 */
}
