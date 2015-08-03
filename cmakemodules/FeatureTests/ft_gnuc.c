int main(int argc, char **argv)
{
#if !defined(_GNUC_) && !defined(__GNUC__)
#pragma error No GNUC extensions available
#error No GNUC extensions available
#endif
  return 0;
}
