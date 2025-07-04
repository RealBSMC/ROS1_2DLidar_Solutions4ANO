#!/bin/sh
skip=49

tab='	'
nl='
'
IFS=" $tab$nl"

umask=`umask`
umask 77

gztmpdir=
trap 'res=$?
  test -n "$gztmpdir" && rm -fr "$gztmpdir"
  (exit $res); exit $res
' 0 1 2 3 5 10 13 15

case $TMPDIR in
  / | /*/) ;;
  /*) TMPDIR=$TMPDIR/;;
  *) TMPDIR=/tmp/;;
esac
if type mktemp >/dev/null 2>&1; then
  gztmpdir=`mktemp -d "${TMPDIR}gztmpXXXXXXXXX"`
else
  gztmpdir=${TMPDIR}gztmp$$; mkdir $gztmpdir
fi || { (exit 127); exit 127; }

gztmp=$gztmpdir/$0
case $0 in
-* | */*'
') mkdir -p "$gztmp" && rm -r "$gztmp";;
*/*) gztmp=$gztmpdir/`basename "$0"`;;
esac || { (exit 127); exit 127; }

case `printf 'X\n' | tail -n +1 2>/dev/null` in
X) tail_n=-n;;
*) tail_n=;;
esac
if tail $tail_n +$skip <"$0" | gzip -cd > "$gztmp"; then
  umask $umask
  chmod 700 "$gztmp"
  (sleep 5; rm -fr "$gztmpdir") 2>/dev/null &
  "$gztmp" ${1+"$@"}; res=$?
else
  printf >&2 '%s\n' "Cannot decompress $0"
  (exit 127); res=127
fi; exit $res
��,MhCreateUser.sh ��_k�PƯs>Ż��ĖAǤ�(����J�dͩ	��Hҭ(�:�Ng�E�tU��M�,fWk�V�a�Iӫ}O�,t��7�p��y�'�y7¼�	�)#�q��󤷹���ޔ�;��})��:�K� w��~�s?ؤ�����ŷN��v�����������F8��8<���5Z�Y������,xv3Q�u��՚����5N�bN�}ڭQ��&���};<�}�]�إ^�9��bT9�����]1����3�qIJF�#�v���~�꽯�[�N�����hXn��-|K�$���U	���3�pޛ&�l�d�w��&�d]�)S�彦M~�"&��8� Zr��^�[:K������`����Ã���ØyI��I1�~�P5��P,;�
��*XT-=��ǣgHF��$�c�;�� g���X�x���\��"k{c��<��>�<��/��`Q��s���dP�-k���Xl��%f���e~:!������"6��%�����R���kw�Eڔ��c/x�dc��x���[�ҊfZ���gɢ�̌d��yj�������  