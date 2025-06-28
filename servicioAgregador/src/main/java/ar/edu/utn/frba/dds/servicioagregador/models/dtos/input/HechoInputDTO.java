package ar.edu.utn.frba.dds.servicioagregador.models.dtos.input;

import ar.edu.utn.frba.dds.servicioagregador.models.dtos.ContenidoMultimediaDto;
import ar.edu.utn.frba.dds.servicioagregador.models.dtos.ContribuyenteDto;
import lombok.Data;

import java.time.LocalDateTime;
import java.util.List;

@Data
public class HechoInputDTO {
  private Long id;
  private String titulo;
  private String descripcion;
  private List<Long> categoriasId;
  private ContenidoMultimediaDto contenidoMultimedia;
  private Double latitud;
  private Double longitud;
  private LocalDateTime fechaAcontecimiento;
  private LocalDateTime fechaDeCarga;
  private String origen;
  private ContribuyenteDto contribuyente;
  private List<Long> etiquetasId;
  private String archivoOrigen;
}
