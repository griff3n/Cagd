#version 330

out uint fragId;
flat in uint fragment_id;

void main()
{
    fragId = fragment_id;
}
