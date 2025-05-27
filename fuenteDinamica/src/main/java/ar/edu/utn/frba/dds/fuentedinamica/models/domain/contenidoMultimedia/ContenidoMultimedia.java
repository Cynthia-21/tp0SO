package ar.edu.utn.frba.dds.fuentedinamica.models.domain.contenidoMultimedia;

import lombok.Setter;

@Setter
public class ContenidoMultimedia {
  String path;
  String descripcion;
  TipoContenido tipoContenido;

  public ContenidoMultimedia(String descripcion) {
    this.path = "";
    this.descripcion = descripcion;
    this.tipoContenido = null;
  }
}