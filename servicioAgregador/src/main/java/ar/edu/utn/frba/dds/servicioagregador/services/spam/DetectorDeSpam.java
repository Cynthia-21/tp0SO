package ar.edu.utn.frba.dds.servicioagregador.services.spam;

public interface DetectorDeSpam {
  boolean esSpam(String texto);
}
