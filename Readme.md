Arduino Library for TZM902DP(ZETA Module)

-------------------------

## TZM902DP(ZETA Module) �Ƃ�

TZM902DP(ZETA Module)�Ƃ́ATOPPAN������� ���甭���� ZETA �ʐM���W���[���ł��B<br>
���̃��W���[���𓋍ڂ��� Spresense ������ Add-on�{�[�h����������AArduino IDE�ł��g�p���\�ł��B

�ڍׂ́A���L���i�T�C�g���������������B

![](http://www.toptdc.com/global-image/units/img/16553-1-20211012174158_b61654a569410b.PNG)

TZM902DP : http://www.toptdc.com/product/zeta/

TOPPAN������ЁFhttps://www.toppan.com/ja/


-------------------------
<PRE>

### API

#### begin

  �����F �Ȃ�

  �߂�l�F �Ȃ�
  
  �����F
	���C�u���������������s���܂��B�B

#### end

  �����F �Ȃ�

  �߂�l�F �Ȃ�
  
  �����F
        ���C�u������j�����܂��B

#### connect

  �����F �Ȃ�
  
  �߂�l�F int wait_time : �o�^�������܂łɑ҂��Ԃł��B
  
  �����F
        AP�ւ��̃��W���[����o�^���ʐM�̏������s���܂��B

#### activate

  �����F �Ȃ�
  
  �߂�l�F bool : ����
  
  �����F
        �ʐM���J�n���邽�߂̃R�}���h�𔭍s���܂��B

#### read

  �����F
          char* store  : ��M�f�[�^���i�[����o�b�t�@�A�h���X
          int size     : ��M�f�[�^���i�[����o�b�t�@�T�C�Y
  
  �߂�l�F int : �ǂݏo���f�[�^�̃T�C�Y
  
  �����F
        ��M�����f�[�^��ǂݏo���܂��B�f�[�^�͈����Ɏw�肵���o�b�t�@�Ɋi�[����܂��B
        �ǂݏo���f�[�^��

#### send

  �����F
          char* var  : ���M����f�[�^�̂̐擪�A�h���X
          int size   : ���M����f�[�^�̃T�C�Y
  
  �߂�l�F bool : ����
  
  �����F
        �f�[�^�𑗐M���܂��B��������܂�3�񑗐M���J��Ԃ��܂��B

#### send_no_retry

  �����F
          char* var  : ���M����f�[�^�̂̐擪�A�h���X
          int size   : ���M����f�[�^�̃T�C�Y
  
  �߂�l�F bool : ����
  
  �����F
        �f�[�^�𑗐M���܂��B���M�����s���Ă��J��Ԃ��܂���B

#### send_raw

  �����F
          char* var  : ���M����p�P�b�g�̂̐擪�A�h���X
          int size   : ���M����p�P�b�g�̃T�C�Y
  
  �߂�l�F bool : ����
  
  �����F
        �p�P�b�g�𑗐M���܂��B�����Ńp�P�b�g�w�b�_�[�𐶐����Ȃ��̂Ŏ��̂��w�肵�Ă��������B
        ���M�����s���Ă��J��Ԃ��܂���B

#### send_raw

  �����F
          char* var  : ���M����p�P�b�g�̂̐擪�A�h���X
          int size   : ���M����p�P�b�g�̃T�C�Y
  
  �߂�l�F bool : ����
  
  �����F
        �p�P�b�g�𑗐M���܂��B�����Ńp�P�b�g�w�b�_�[�𐶐����Ȃ��̂Ŏ��̂��w�肵�Ă��������B
        ���M�����s���Ă��J��Ԃ��܂���B

#### downlink_check

  �����F �Ȃ�

  
  �߂�l�F int : �����܂��̓_�E�������N�p�P�b�g�̃^�C�v
  
  �����F
        ZETA���W���[�����牞���܂��̓_�E�������N���擾���܂��B

#### get_downlink

  �����F
          char* data  : �_�E�������N�f�[�^���i�[����o�b�t�@�A�h���X
          int size    : �_�E�������N�f�[�^���i�[����o�b�t�@�T�C�Y
  
  �߂�l�F int : �_�E�������N�f�[�^�̃T�C�Y
  
  �����F
        �_�E�������N�����f�[�^��ǂݏo���܂��B�f�[�^�͈����Ɏw�肵���o�b�t�@�Ɋi�[����܂��B
        �_�E�������N�f�[�^�̃T�C�Y���߂�܂��B

### �g����

  �ȉ��̃T���v���R�[�h���Q�l�ɂ��Ă��������B

    examples/LowPower_Temperature_Sensing

</PRE>

